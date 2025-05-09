﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <pch.h>
#include <common.h>
#include <ItemsRepeater.common.h>
#include "FlowLayoutAlgorithm.h"
#include "VirtualizingLayoutContext.h"
#include "MuxcTraceLogging.h"

void FlowLayoutAlgorithm::InitializeForContext(
    const winrt::VirtualizingLayoutContext& context,
    IFlowLayoutAlgorithmDelegates* callbacks)
{
    m_algorithmCallbacks = callbacks;
    m_context.set(context);
    m_elementManager.SetContext(context);
}

void FlowLayoutAlgorithm::UninitializeForContext(
    const winrt::VirtualizingLayoutContext& context)
{
    if (IsVirtualizingContext())
    {
        // This layout is about to be detached. Let go of all elements
        // being held and remove the layout state from the context.
        m_elementManager.ClearRealizedRange();
    }
    context.LayoutStateCore(nullptr);
}

winrt::Size FlowLayoutAlgorithm::Measure(
    const winrt::Size& availableSize,
    const winrt::VirtualizingLayoutContext& context,
    bool isWrapping,
    double minItemSpacing,
    double lineSpacing,
    unsigned int maxItemsPerLine,
    const ScrollOrientation& orientation,
    const bool isVirtualizationEnabled,
    const wstring_view& layoutId)
{
    SetScrollOrientation(orientation);

    // If minor size is infinity, there is only one line and no need to align that line.
    m_scrollOrientationSameAsFlow = Minor(availableSize) == std::numeric_limits<float>::infinity();
    const auto realizationRect = RealizationRect();

    ITEMSREPEATER_TRACE_INFO_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_FLT_FLT_FLT_FLT, METH_NAME, this,
        winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
        L"MeasureLayout Realization:", realizationRect.X, realizationRect.Y, realizationRect.Width, realizationRect.Height);

    TraceLoggingProviderWrite(
        XamlTelemetryLogging, "FlowLayoutAlgorithm_Measure",
        TraceLoggingWideString(layoutId.data(), "LayoutId"),
        TraceLoggingFloat32(realizationRect.X, "X"),
        TraceLoggingFloat32(realizationRect.Y, "Y"),
        TraceLoggingFloat32(realizationRect.Width, "Width"),
        TraceLoggingFloat32(realizationRect.Height, "Height"),
        TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));

    const auto suggestedAnchorIndex = m_context.get().RecommendedAnchorIndex();
    if (m_elementManager.IsIndexValidInData(suggestedAnchorIndex))
    {
        const auto anchorRealized = m_elementManager.IsDataIndexRealized(suggestedAnchorIndex);
        if (!anchorRealized)
        {
            MakeAnchor(m_context.get(), suggestedAnchorIndex, availableSize);
        }
    }

    if (isVirtualizationEnabled)
    {
        m_elementManager.OnBeginMeasure(orientation);
    }

    const int anchorIndex = GetAnchorIndex(availableSize, isWrapping, minItemSpacing, isVirtualizationEnabled, layoutId);
    Generate(GenerateDirection::Forward, anchorIndex, availableSize, isWrapping, minItemSpacing, lineSpacing, maxItemsPerLine, isVirtualizationEnabled, layoutId);
    Generate(GenerateDirection::Backward, anchorIndex, availableSize, isWrapping, minItemSpacing, lineSpacing, maxItemsPerLine, isVirtualizationEnabled, layoutId);
    if (isWrapping && IsReflowRequired())
    {
        ITEMSREPEATER_TRACE_INFO_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
            winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(), L"Reflow Pass.");

        const auto firstElementBounds = m_elementManager.GetLayoutBoundsForRealizedIndex(0);
        MinorStart(firstElementBounds) = 0;
        m_elementManager.SetLayoutBoundsForRealizedIndex(0, firstElementBounds);
        Generate(GenerateDirection::Forward, 0 /*anchorIndex*/, availableSize, isWrapping, minItemSpacing, lineSpacing, maxItemsPerLine, isVirtualizationEnabled, layoutId);
    }

    RaiseLineArranged();
    m_collectionChangePending = false;
    const winrt::Rect lastExtent = EstimateExtent(availableSize, layoutId);

    // When m_layoutRoundFactor was set, the layout origin and extent are rounded based on
    // that factor to avoid accumulations of double-to-float rounding imprecisions.
    m_lastExtent = m_layoutRoundFactor > 0.0 ? LayoutRound(lastExtent) : lastExtent;
    SetLayoutOrigin();

#ifdef DBG
    ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
        winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(), L"ElementManager:");

    m_elementManager.LogElementManagerDbg(winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data());
#endif // DBG

    return winrt::Size{ m_lastExtent.Width, m_lastExtent.Height };
}

winrt::Size FlowLayoutAlgorithm::Arrange(
    const winrt::Size& finalSize,
    const winrt::VirtualizingLayoutContext& context,
    bool isWrapping,
    FlowLayoutAlgorithm::LineAlignment lineAlignment,
    const wstring_view& layoutId)
{
#ifdef DBG
    ITEMSREPEATER_TRACE_INFO(nullptr, TRACE_MSG_METH_IND_STR_STR_FLT_FLT, METH_NAME, this,
        winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
        L"ArrangeLayout LayoutOrigin:", context.LayoutOrigin().X, context.LayoutOrigin().Y);

    MUX_ASSERT(m_lastExtent.X == context.LayoutOrigin().X);
    MUX_ASSERT(m_lastExtent.Y == context.LayoutOrigin().Y);

    const auto realizationRectDbg = RealizationRect();

    ITEMSREPEATER_TRACE_INFO(nullptr, TRACE_MSG_METH_IND_STR_STR_FLT_FLT_FLT_FLT, METH_NAME, this,
        winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
        L"ArrangeLayout Realization:", realizationRectDbg.X, realizationRectDbg.Y, realizationRectDbg.Width, realizationRectDbg.Height);
#endif

    ArrangeVirtualizingLayout(finalSize, lineAlignment, isWrapping, layoutId);

    return winrt::Size
    {
        std::max(finalSize.Width, m_lastExtent.Width),
        std::max(finalSize.Height, m_lastExtent.Height)
    };
}

void FlowLayoutAlgorithm::MakeAnchor(
    const winrt::VirtualizingLayoutContext& context,
    int index,
    const winrt::Size& availableSize)
{
    m_elementManager.ClearRealizedRange();
    // FlowLayout requires that the anchor is the first element in the row.
    const auto internalAnchor = m_algorithmCallbacks->Algorithm_GetAnchorForTargetElement(index, availableSize, context);
    MUX_ASSERT(internalAnchor.Index <= index);

    // No need to set the position of the anchor.
    // (0,0) is fine for now since the extent can
    // grow in any direction.
    for (int dataIndex = internalAnchor.Index; dataIndex < index + 1; ++dataIndex)
    {
        auto element = context.GetOrCreateElementAt(dataIndex, winrt::ElementRealizationOptions::ForceCreate | winrt::ElementRealizationOptions::SuppressAutoRecycle);
        element.Measure(m_algorithmCallbacks->Algorithm_GetMeasureSize(dataIndex, availableSize, context));
        m_elementManager.Add(element, dataIndex);
    }
}

void FlowLayoutAlgorithm::OnItemsSourceChanged(
    const winrt::IInspectable& source,
    winrt::NotifyCollectionChangedEventArgs const& args,
    const winrt::IVirtualizingLayoutContext& /*context*/)
{
    m_elementManager.DataSourceChanged(source, args);
    m_collectionChangePending = true;
}

winrt::Size FlowLayoutAlgorithm::MeasureElement(
    const winrt::UIElement& element,
    int index,
    const winrt::Size& availableSize,
    const winrt::VirtualizingLayoutContext& context)
{
    // Using the handy 'element' to determine the global scale factor because accessing the ItemsRepeater panel at this FlowLayoutAlgorithm level
    // would require significant additions. This scale factor is used to round the layout's origin and extent.
    EvaluateLayoutRoundFactor(context, element);

    const auto measureSize = m_algorithmCallbacks->Algorithm_GetMeasureSize(index, availableSize, context);
    element.Measure(measureSize);
    const auto desiredSize = element.DesiredSize();
    const auto provisionalArrangeSize = m_algorithmCallbacks->Algorithm_GetProvisionalArrangeSize(index, measureSize, desiredSize, context);

    m_algorithmCallbacks->Algorithm_OnElementMeasured(element, index, availableSize, measureSize, desiredSize, provisionalArrangeSize, context);

    return provisionalArrangeSize;
}

#pragma region Measure related private methods

int FlowLayoutAlgorithm::GetAnchorIndex(
    const winrt::Size& availableSize,
    bool isWrapping,
    double minItemSpacing,
    const bool isVirtualizationEnabled,
    const wstring_view& layoutId)
{
    int anchorIndex = -1;
    winrt::Point anchorPosition{};
    auto context = m_context.get();

    if (!IsVirtualizingContext() || !isVirtualizationEnabled)
    {
        // Non virtualizing host, start generating from the element 0
        if (context.ItemCountCore() > 0)
        {
            anchorIndex = 0;
        }
        else
        {
            anchorPosition = { -1.0, -1.0 };
        }
    }
    else
    {
        const bool isRealizationWindowConnected = m_elementManager.IsWindowConnected(RealizationRect(), GetScrollOrientation(), m_scrollOrientationSameAsFlow);

        ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
            winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(), isRealizationWindowConnected ? L"Connected RealizationWindow." : L"Disconnected RealizationWindow.");

        // Item spacing and size in non-virtualizing direction change can cause elements to reflow
        // and get a new column position. In that case we need the anchor to be positioned in the
        // correct column.
        const bool needAnchorColumnReevaluation = isWrapping && (
            Minor(m_lastAvailableSize) != Minor(availableSize) ||
            m_lastItemSpacing != minItemSpacing ||
            m_collectionChangePending);

        const auto suggestedAnchorIndex = m_context.get().RecommendedAnchorIndex();

        const bool isAnchorSuggestionValid = suggestedAnchorIndex >= 0 &&
            m_elementManager.IsDataIndexRealized(suggestedAnchorIndex);

        if (isAnchorSuggestionValid)
        {
            ITEMSREPEATER_TRACE_INFO_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_INT, METH_NAME, this,
                winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
                L"Using suggested anchor:", suggestedAnchorIndex);

            anchorIndex = m_algorithmCallbacks->Algorithm_GetAnchorForTargetElement(
                suggestedAnchorIndex,
                availableSize,
                context).Index;

            if (m_elementManager.IsDataIndexRealized(anchorIndex))
            {
                const auto anchorBounds = m_elementManager.GetLayoutBoundsForDataIndex(anchorIndex);
                if (needAnchorColumnReevaluation)
                {
                    // We were provided a valid anchor, but its position might be incorrect because for example it is in
                    // the wrong column. We do know that the anchor is the first element in the row, so we can force the minor position
                    // to start at 0.
                    anchorPosition = MinorMajorPoint(0, MajorStart(anchorBounds));
                }
                else
                {
                    anchorPosition = winrt::Point(anchorBounds.X, anchorBounds.Y);
                }
            }
            else
            {
                // It is possible to end up in a situation during a collection change where GetAnchorForTargetElement returns an index
                // which is not in the realized range. Eg. insert one item at index 0 for a grid layout.
                // SuggestedAnchor will be 1 (used to be 0) and GetAnchorForTargetElement will return 0 (left most item in row). However 0 is not in the
                // realized range yet. In this case we realize the gap between the target anchor and the suggested anchor.
                const int firstRealizedDataIndex = m_elementManager.GetDataIndexFromRealizedRangeIndex(0);
                MUX_ASSERT(anchorIndex < firstRealizedDataIndex);
                for (int i = firstRealizedDataIndex - 1; i >= anchorIndex; --i)
                {
                    m_elementManager.EnsureElementRealized(false /*forward*/, i, layoutId);
                }

                const auto anchorBounds = m_elementManager.GetLayoutBoundsForDataIndex(suggestedAnchorIndex);
                anchorPosition = MinorMajorPoint(0, MajorStart(anchorBounds));
            }
        }
        else if (needAnchorColumnReevaluation || !isRealizationWindowConnected)
        {
            if (needAnchorColumnReevaluation)
            {
                ITEMSREPEATER_TRACE_INFO_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
                    winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(), L"NeedAnchorColumnReevaluation.");
            }

            // The anchor is based on the realization window because a connected ItemsRepeater might intersect the realization window
            // but not the visible window. In that situation, we still need to produce a valid anchor.
            const auto anchorInfo = m_algorithmCallbacks->Algorithm_GetAnchorForRealizationRect(availableSize, context);
            anchorIndex = anchorInfo.Index;
            anchorPosition = MinorMajorPoint(0, static_cast<float>(anchorInfo.Offset));
        }
        else
        {
            ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
                winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
                L"Connected Window - picking first realized element as anchor.");

            // No suggestion - just pick first in realized range
            anchorIndex = m_elementManager.GetDataIndexFromRealizedRangeIndex(0);

            if (m_elementManager.IsLayoutBoundsForRealizedIndexSet(0))
            {
                const auto firstRealizedElementBounds = m_elementManager.GetLayoutBoundsForRealizedIndex(0);

                anchorPosition = MinorMajorPoint(MinorStart(firstRealizedElementBounds), MajorStart(firstRealizedElementBounds));
            }
        }
    }

    ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_INT, METH_NAME, this,
        winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
        L"Picked anchor:", anchorIndex);

    MUX_ASSERT(anchorIndex == -1 || m_elementManager.IsIndexValidInData(anchorIndex));
    m_firstRealizedDataIndexInsideRealizationWindow = m_lastRealizedDataIndexInsideRealizationWindow = anchorIndex;
    if (m_elementManager.IsIndexValidInData(anchorIndex))
    {
        if (!m_elementManager.IsDataIndexRealized(anchorIndex))
        {
            // Disconnected, throw everything and create new anchor
            ITEMSREPEATER_TRACE_INFO_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
                winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
                L"Disconnected Window - throwing away all realized elements.");

            m_elementManager.ClearRealizedRange();

            auto anchor = m_context.get().GetOrCreateElementAt(anchorIndex, winrt::ElementRealizationOptions::ForceCreate | winrt::ElementRealizationOptions::SuppressAutoRecycle);
            m_elementManager.Add(anchor, anchorIndex);
        }

        const auto anchorElement = m_elementManager.GetRealizedElement(anchorIndex);
        const auto desiredSize = MeasureElement(anchorElement, anchorIndex, availableSize, m_context.get());
        const auto layoutBounds = winrt::Rect{ anchorPosition.X, anchorPosition.Y, desiredSize.Width, desiredSize.Height };
        m_elementManager.SetLayoutBoundsForDataIndex(anchorIndex, layoutBounds);

        ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_INT_FLT_FLT_FLT_FLT, METH_NAME, this,
            winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
            L"Anchor index & Layout bounds:", anchorIndex, layoutBounds.X, layoutBounds.Y, layoutBounds.Width, layoutBounds.Height);
    }
    else
    {
        // Throw everything away
        ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR, METH_NAME, this,
            winrt::get_self<VirtualizingLayoutContext>(context)->Indent(), layoutId.data(),
            L"Anchor index is not valid - throwing away all realized elements.");
        m_elementManager.ClearRealizedRange();
    }

    // TODO: Perhaps we can track changes in the property setter
    m_lastAvailableSize = availableSize;
    m_lastItemSpacing = minItemSpacing;

#ifdef DBG
    m_algorithmCallbacks->Algorithm_SetFlowLayoutAnchorInfoDbg(anchorIndex, Major(anchorPosition));
#endif // DBG

    return anchorIndex;
}

void FlowLayoutAlgorithm::Generate(
    GenerateDirection direction,
    int anchorIndex,
    const winrt::Size& availableSize,
    bool isWrapping,
    double minItemSpacing,
    double lineSpacing,
    unsigned int maxItemsPerLine,
    const bool isVirtualizationEnabled,
    const wstring_view& layoutId)
{
    if (anchorIndex != -1)
    {
        const int step = (direction == GenerateDirection::Forward) ? 1 : -1;

        ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_INT, METH_NAME, this,
            winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
            direction == GenerateDirection::Forward ? L"Generating forward from anchor:" : L"Generating backward from anchor:",
            anchorIndex);

        TraceLoggingProviderWrite(
            XamlTelemetryLogging, "FlowLayoutAlgorithm_Generate",
            TraceLoggingWideString(layoutId.data(), "LayoutId"),
            TraceLoggingWideString(direction == GenerateDirection::Forward ? L"Forward" : L"Backward", "Direction"),
            TraceLoggingInt32(anchorIndex, "Anchor"),
            TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));

        int previousIndex = anchorIndex;
        int currentIndex = anchorIndex + step;
        const auto anchorBounds = m_elementManager.GetLayoutBoundsForDataIndex(anchorIndex);
        float lineOffset = MajorStart(anchorBounds);
        float lineMajorSize = MajorSize(anchorBounds);
        unsigned int countInLine = 1;
        bool lineNeedsReposition = false;

        if (maxItemsPerLine > 0 && maxItemsPerLine != std::numeric_limits<unsigned int>::max())
        {
            // Initialize countInLine based on anchorIndex & maxItemsPerLine for a UniformGridLayout for example.
            const int anchorLineIndex = static_cast<int>(anchorIndex / maxItemsPerLine);
            const int anchorIndexInLine = anchorIndex - (anchorLineIndex * maxItemsPerLine);
            countInLine = direction == GenerateDirection::Forward ? anchorIndexInLine + 1 : maxItemsPerLine - anchorIndexInLine;
        }

        while (m_elementManager.IsIndexValidInData(currentIndex) &&
              (!isVirtualizationEnabled || ShouldContinueFillingUpSpace(previousIndex, direction)))
        {
            // Ensure layout element.
            m_elementManager.EnsureElementRealized(direction == GenerateDirection::Forward, currentIndex, layoutId);
            const auto currentElement = m_elementManager.GetRealizedElement(currentIndex);
            const auto desiredSize = MeasureElement(currentElement, currentIndex, availableSize, m_context.get());

            // Lay it out.
            const auto previousElement = m_elementManager.GetRealizedElement(previousIndex);
            const winrt::Rect currentBounds = winrt::Rect{ 0, 0, desiredSize.Width, desiredSize.Height };
            const auto previousElementBounds = m_elementManager.GetLayoutBoundsForDataIndex(previousIndex);

            TraceLoggingProviderWrite(
                XamlTelemetryLogging, "FlowLayoutAlgorithm_Generate_CurrentBounds",
                TraceLoggingWideString(layoutId.data(), "LayoutId"),
                TraceLoggingInt32(currentIndex, "CurrentIndex"),
                TraceLoggingFloat32(currentBounds.X, "X"),
                TraceLoggingFloat32(currentBounds.Y, "Y"),
                TraceLoggingFloat32(currentBounds.Width, "Width"),
                TraceLoggingFloat32(currentBounds.Height, "Height"),
                TraceLoggingLevel(WINEVENT_LEVEL_VERBOSE));

            if (direction == GenerateDirection::Forward)
            {
                const double remainingSpace = static_cast<double>(Minor(availableSize)) -
                                              (static_cast<double>(MinorStart(previousElementBounds)) +
                                               static_cast<double>(MinorSize(previousElementBounds)) +
                                               minItemSpacing +
                                               static_cast<double>(Minor(desiredSize)));
                if (countInLine >= maxItemsPerLine || m_algorithmCallbacks->Algorithm_ShouldBreakLine(currentIndex, remainingSpace))
                {
                    // No more space in this row. wrap to next row.
                    MinorStart(currentBounds) = 0;
                    MajorStart(currentBounds) = MajorStart(previousElementBounds) + lineMajorSize + static_cast<float>(lineSpacing);

                    if (lineNeedsReposition)
                    {
                        // reposition the previous line (countInLine items)
                        for (unsigned int i = 0; i < countInLine; i++)
                        {
                            const auto dataIndex = currentIndex - 1 - i;
                            const auto bounds = m_elementManager.GetLayoutBoundsForDataIndex(dataIndex);
                            MajorSize(bounds) = lineMajorSize;
                            m_elementManager.SetLayoutBoundsForDataIndex(dataIndex, bounds);
                        }
                    }

                    // Setup for next line.
                    lineMajorSize = MajorSize(currentBounds);
                    lineOffset = MajorStart(currentBounds);
                    lineNeedsReposition = false;
                    countInLine = 1;
                }
                else
                {
                    // More space is available in this row.
                    MinorStart(currentBounds) = MinorStart(previousElementBounds) + MinorSize(previousElementBounds) + static_cast<float>(minItemSpacing);
                    MajorStart(currentBounds) = lineOffset;
                    lineMajorSize = std::max(lineMajorSize, MajorSize(currentBounds));
                    lineNeedsReposition = MajorSize(previousElementBounds) != MajorSize(currentBounds);
                    countInLine++;
                }
            }
            else
            {
                // Backward
                const double remainingSpace = MinorStart(previousElementBounds) - (Minor(desiredSize) + minItemSpacing);
                if (countInLine >= maxItemsPerLine || m_algorithmCallbacks->Algorithm_ShouldBreakLine(currentIndex, remainingSpace))
                {
                    if (isWrapping)
                    {
                        // Does not fit, wrap to the previous row
                        const auto availableSizeMinor = Minor(availableSize);
                        // If the last available size is finite, start from end and subtract our desired size.
                        // Otherwise, look at the last extent and use that for positioning.
                        MinorStart(currentBounds) = std::isfinite(availableSizeMinor) ? availableSizeMinor - Minor(desiredSize) : MinorSize(LastExtent()) - Minor(desiredSize);
                    }
                    // else keep MinorStart(currentBounds) at 0. Same as for GenerateDirection::Forward.

                    MajorStart(currentBounds) = lineOffset - Major(desiredSize) - static_cast<float>(lineSpacing);

                    if (lineNeedsReposition)
                    {
                        auto previousLineOffset = MajorStart(m_elementManager.GetLayoutBoundsForDataIndex(currentIndex + countInLine + 1));
                        // reposition the previous line (countInLine items)
                        for (unsigned int i = 0; i < countInLine; i++)
                        {
                            const auto dataIndex = currentIndex + 1 + (int)i;
                            if (dataIndex != anchorIndex)
                            {
                                const auto bounds = m_elementManager.GetLayoutBoundsForDataIndex(dataIndex);
                                MajorStart(bounds) = previousLineOffset - lineMajorSize - static_cast<float>(lineSpacing);
                                MajorSize(bounds) = lineMajorSize;
                                m_elementManager.SetLayoutBoundsForDataIndex(dataIndex, bounds);

                                ITEMSREPEATER_TRACE_INFO_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_INT_FLT_FLT_FLT_FLT, METH_NAME, this,
                                    winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
                                    L"Element dataIndex & corrected layout bounds:", dataIndex, bounds.X, bounds.Y, bounds.Width, bounds.Height);
                            }
                        }
                    }

                    // Setup for next line.
                    lineMajorSize = MajorSize(currentBounds);
                    lineOffset = MajorStart(currentBounds);
                    lineNeedsReposition = false;
                    countInLine = 1;
                }
                else
                {
                    // Fits in this row. put it in the previous position
                    MinorStart(currentBounds) = MinorStart(previousElementBounds) - Minor(desiredSize) - static_cast<float>(minItemSpacing);
                    MajorStart(currentBounds) = lineOffset;
                    lineMajorSize = std::max(lineMajorSize, MajorSize(currentBounds));
                    lineNeedsReposition = MajorSize(previousElementBounds) != MajorSize(currentBounds);
                    countInLine++;
                }
            }

            m_elementManager.SetLayoutBoundsForDataIndex(currentIndex, currentBounds);

            ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_INT_FLT_FLT_FLT_FLT, METH_NAME, this,
                winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
                L"Element dataIndex & Layout bounds:", currentIndex, currentBounds.X, currentBounds.Y, currentBounds.Width, currentBounds.Height);

            previousIndex = currentIndex;
            currentIndex += step;
        }

        const int dataCount = m_context.get().ItemCount();

        // If we did not reach the top or bottom of the extent, we realized one
        // extra item before we knew we were outside the realization window. Do not
        // account for that element in the indices inside the realization window.
        if (direction == GenerateDirection::Forward)
        {
            m_lastRealizedDataIndexInsideRealizationWindow = previousIndex == dataCount - 1 ? dataCount - 1 : previousIndex - 1;
            m_lastRealizedDataIndexInsideRealizationWindow = std::max(0, m_lastRealizedDataIndexInsideRealizationWindow);
        }
        else
        {
            m_firstRealizedDataIndexInsideRealizationWindow = previousIndex == 0 ? 0 : previousIndex + 1;
            m_firstRealizedDataIndexInsideRealizationWindow = std::min(dataCount - 1, m_firstRealizedDataIndexInsideRealizationWindow);
        }

        m_elementManager.DiscardElementsOutsideWindow(direction == GenerateDirection::Forward, currentIndex);
    }
}

bool FlowLayoutAlgorithm::IsReflowRequired() const
{
    // If first element is realized and is not at the very beginning we need to reflow.
    return
        m_elementManager.GetRealizedElementCount() > 0 &&
        m_elementManager.GetDataIndexFromRealizedRangeIndex(0) == 0 &&
        (GetScrollOrientation() == ScrollOrientation::Vertical ?
        m_elementManager.GetLayoutBoundsForRealizedIndex(0).X != 0 :
        m_elementManager.GetLayoutBoundsForRealizedIndex(0).Y != 0);
}

bool FlowLayoutAlgorithm::ShouldContinueFillingUpSpace(
    int index,
    GenerateDirection direction)
{
    bool shouldContinue = false;
    if (!IsVirtualizingContext())
    {
        shouldContinue = true;
    }
    else
    {
        const auto realizationRect = m_context.get().RealizationRect();
        const auto elementBounds = m_elementManager.GetLayoutBoundsForDataIndex(index);

        const auto elementMajorStart = MajorStart(elementBounds);
        const auto elementMajorEnd = MajorEnd(elementBounds);
        const auto rectMajorStart = MajorStart(realizationRect);
        const auto rectMajorEnd = MajorEnd(realizationRect);

        const auto elementMinorStart = MinorStart(elementBounds);
        const auto elementMinorEnd = MinorEnd(elementBounds);
        const auto rectMinorStart = MinorStart(realizationRect);
        const auto rectMinorEnd = MinorEnd(realizationRect);

#ifdef DBG
        if (direction == GenerateDirection::Forward)
        {
            ITEMSREPEATER_TRACE_VERBOSE(nullptr, TRACE_MSG_METH_STR_FLT_FLT, METH_NAME, this,
                L"elementMajorStart < rectMajorEnd ?", elementMajorStart, rectMajorEnd);
        }
        else
        {
            ITEMSREPEATER_TRACE_VERBOSE(nullptr, TRACE_MSG_METH_STR_FLT_FLT, METH_NAME, this,
                L"elementMajorEnd > rectMajorStart ?", elementMajorEnd, rectMajorStart);
        }
#endif // DBG

        // Ensure that both minor and major directions are taken into consideration so that if the scrolling direction
        // is the same as the flow direction we still stop at the end of the viewport rectangle.
        shouldContinue = direction == GenerateDirection::Forward
            ? elementMajorStart < rectMajorEnd && elementMinorStart < rectMinorEnd
            : elementMajorEnd > rectMajorStart && elementMinorEnd > rectMinorStart;
    }

    return shouldContinue;
}

winrt::Rect FlowLayoutAlgorithm::EstimateExtent(
    const winrt::Size& availableSize,
    const wstring_view& layoutId)
{
    winrt::UIElement firstRealizedElement = nullptr;
    winrt::Rect firstRealizedBounds{};
    winrt::UIElement lastRealizedElement = nullptr;
    winrt::Rect lastRealizedBounds{};
    int firstRealizedDataIndex = -1;
    int lastRealizedDataIndex = -1;

    if (m_elementManager.GetRealizedElementCount() > 0)
    {
        firstRealizedElement = m_elementManager.GetAt(0);
        firstRealizedBounds = m_elementManager.GetLayoutBoundsForRealizedIndex(0);
        firstRealizedDataIndex = m_elementManager.GetDataIndexFromRealizedRangeIndex(0);

        const int last = m_elementManager.GetRealizedElementCount() - 1;
        lastRealizedElement = m_elementManager.GetAt(last);
        lastRealizedDataIndex = m_elementManager.GetDataIndexFromRealizedRangeIndex(last);
        lastRealizedBounds = m_elementManager.GetLayoutBoundsForRealizedIndex(last);
    }

    const winrt::Rect extent = m_algorithmCallbacks->Algorithm_GetExtent(
        availableSize,
        m_context.get(),
        firstRealizedElement,
        firstRealizedDataIndex,
        firstRealizedBounds,
        lastRealizedElement,
        lastRealizedDataIndex,
        lastRealizedBounds);

    ITEMSREPEATER_TRACE_VERBOSE_DBG(nullptr, TRACE_MSG_METH_IND_STR_STR_FLT_FLT_FLT_FLT, METH_NAME, this,
        winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
        L"Extent:", extent.X, extent.Y, extent.Width, extent.Height);

    return extent;
}

winrt::Rect FlowLayoutAlgorithm::LayoutRound(
    const winrt::Rect& value) const
{
    MUX_ASSERT(m_layoutRoundFactor != 0.0);

    return winrt::Rect
    {
        static_cast<float>(round(value.X * m_layoutRoundFactor) / m_layoutRoundFactor),
        static_cast<float>(round(value.Y * m_layoutRoundFactor) / m_layoutRoundFactor),
        static_cast<float>(round(value.Width * m_layoutRoundFactor) / m_layoutRoundFactor),
        static_cast<float>(round(value.Height * m_layoutRoundFactor) / m_layoutRoundFactor)
    };
}

void FlowLayoutAlgorithm::EvaluateLayoutRoundFactor(
    const winrt::VirtualizingLayoutContext& context,
    const winrt::UIElement& element)
{
    if (element.UseLayoutRounding())
    {
        if (const auto xamlRoot = element.XamlRoot())
        {
            const double layoutRoundFactor = xamlRoot.RasterizationScale();

            if (layoutRoundFactor != m_layoutRoundFactor)
            {
                m_layoutRoundFactor = layoutRoundFactor;

                // This triggers a call to StackLayoutState::OnElementSizesReset()
                // for example, in the StackLayout case.
                m_algorithmCallbacks->Algorithm_OnLayoutRoundFactorChanged(context);
            }
        }
        else
        {
            m_layoutRoundFactor = 0.0;
        }
    }
    else
    {
        m_layoutRoundFactor = 0.0;
    }
}

void FlowLayoutAlgorithm::RaiseLineArranged()
{
    const auto realizationRect = RealizationRect();
    if (realizationRect.Width != 0.0f || realizationRect.Height != 0.0f)
    {
        const int realizedElementCount = m_elementManager.GetRealizedElementCount();
        if (realizedElementCount > 0)
        {
            MUX_ASSERT(m_firstRealizedDataIndexInsideRealizationWindow != -1 && m_lastRealizedDataIndexInsideRealizationWindow != -1);
            int countInLine = 0;
            auto previousElementBounds = m_elementManager.GetLayoutBoundsForDataIndex(m_firstRealizedDataIndexInsideRealizationWindow);
            auto currentLineOffset = MajorStart(previousElementBounds);
            auto currentLineSize = MajorSize(previousElementBounds);
            for (int currentDataIndex = m_firstRealizedDataIndexInsideRealizationWindow; currentDataIndex <= m_lastRealizedDataIndexInsideRealizationWindow; currentDataIndex++)
            {
                const auto currentBounds = m_elementManager.GetLayoutBoundsForDataIndex(currentDataIndex);
                if (MajorStart(currentBounds) != currentLineOffset)
                {
                    // Starting a new line
                    m_algorithmCallbacks->Algorithm_OnLineArranged(currentDataIndex - countInLine, countInLine, currentLineSize, m_context.get());
                    countInLine = 0;
                    currentLineOffset = MajorStart(currentBounds);
                    currentLineSize = 0;
                }

                currentLineSize = std::max(static_cast<float>(currentLineSize), MajorSize(currentBounds));
                countInLine++;
                previousElementBounds = currentBounds;
            }

            // Raise for the last line.
            m_algorithmCallbacks->Algorithm_OnLineArranged(m_lastRealizedDataIndexInsideRealizationWindow - countInLine + 1, countInLine, currentLineSize, m_context.get());
        }
    }
}

#pragma endregion

#pragma region Arrange related private methods

void FlowLayoutAlgorithm::ArrangeVirtualizingLayout(
    const winrt::Size& finalSize,
    FlowLayoutAlgorithm::LineAlignment lineAlignment,
    bool isWrapping,
    const wstring_view& layoutId)
{
    // Walk through the realized elements one line at a time and
    // align them. Then call element.Arrange with the arranged bounds.
    const int realizedElementCount = m_elementManager.GetRealizedElementCount();

    if (realizedElementCount > 0)
    {
        int countInLine = 1;
        auto previousElementBounds = m_elementManager.GetLayoutBoundsForRealizedIndex(0);
        auto currentLineOffset = MajorStart(previousElementBounds);
        auto spaceAtLineStart = MinorStart(previousElementBounds);
        float spaceAtLineEnd = 0;
        float currentLineSize = MajorSize(previousElementBounds);

        for (int i = 1; i < realizedElementCount; i++)
        {
            const auto currentBounds = m_elementManager.GetLayoutBoundsForRealizedIndex(i);

            if (MajorStart(currentBounds) != currentLineOffset)
            {
                spaceAtLineEnd = Minor(finalSize) - MinorStart(previousElementBounds) - MinorSize(previousElementBounds);
                PerformLineAlignment(i - countInLine, countInLine, spaceAtLineStart, spaceAtLineEnd, currentLineSize, lineAlignment, isWrapping, finalSize, layoutId);
                spaceAtLineStart = MinorStart(currentBounds);
                countInLine = 0;
                currentLineOffset = MajorStart(currentBounds);
                currentLineSize = 0;
            }

            countInLine++; // for current element
            currentLineSize = std::max(currentLineSize, MajorSize(currentBounds));
            previousElementBounds = currentBounds;
        }

        // Last line - potentially have a property to customize
        // aligning the last line or not.
        if (countInLine > 0)
        {
            const float spaceAtEnd = Minor(finalSize) - MinorStart(previousElementBounds) - MinorSize(previousElementBounds);

            PerformLineAlignment(realizedElementCount - countInLine, countInLine, spaceAtLineStart, spaceAtEnd, currentLineSize, lineAlignment, isWrapping, finalSize, layoutId);
        }
    }
}

// Align elements within a line. Note that this does not modify LayoutBounds. So if we get
// repeated measures, the LayoutBounds remain the same in each layout.
void FlowLayoutAlgorithm::PerformLineAlignment(
    int lineStartIndex,
    int countInLine,
    float spaceAtLineStart,
    float spaceAtLineEnd,
    float lineSize,
    FlowLayoutAlgorithm::LineAlignment lineAlignment,
    bool isWrapping,
    const winrt::Size& finalSize,
    const wstring_view& layoutId)
{
    for (int rangeIndex = lineStartIndex; rangeIndex < lineStartIndex + countInLine; ++rangeIndex)
    {
        auto bounds = m_elementManager.GetLayoutBoundsForRealizedIndex(rangeIndex);
        MajorSize(bounds) = lineSize;

        if (!m_scrollOrientationSameAsFlow)
        {
            // Note: Space at start could potentially be negative
            if (spaceAtLineStart != 0 || spaceAtLineEnd != 0)
            {
                float totalSpace = spaceAtLineStart + spaceAtLineEnd;
                switch (lineAlignment)
                {
                case FlowLayoutAlgorithm::LineAlignment::Start:
                {
                    MinorStart(bounds) -= spaceAtLineStart;
                    break;
                }

                case FlowLayoutAlgorithm::LineAlignment::End:
                {
                    MinorStart(bounds) += spaceAtLineEnd;
                    break;
                }

                case FlowLayoutAlgorithm::LineAlignment::Center:
                {
                    MinorStart(bounds) -= spaceAtLineStart;
                    MinorStart(bounds) += totalSpace / 2;
                    break;
                }

                case FlowLayoutAlgorithm::LineAlignment::SpaceAround:
                {
                    float interItemSpace = countInLine >= 1 ? totalSpace / (countInLine * 2) : 0;
                    MinorStart(bounds) -= spaceAtLineStart;
                    MinorStart(bounds) += interItemSpace * ((rangeIndex - lineStartIndex + 1) * 2 - 1);
                    break;
                }

                case FlowLayoutAlgorithm::LineAlignment::SpaceBetween:
                {
                    float interItemSpace = countInLine > 1 ? totalSpace / (countInLine - 1) : 0;
                    MinorStart(bounds) -= spaceAtLineStart;
                    MinorStart(bounds) += interItemSpace * (rangeIndex - lineStartIndex);
                    break;
                }

                case FlowLayoutAlgorithm::LineAlignment::SpaceEvenly:
                {
                    float interItemSpace = countInLine >= 1 ? totalSpace / (countInLine + 1) : 0;
                    MinorStart(bounds) -= spaceAtLineStart;
                    MinorStart(bounds) += interItemSpace * (rangeIndex - lineStartIndex + 1);
                    break;
                }
                }
            }
        }

        bounds.X -= m_lastExtent.X;
        bounds.Y -= m_lastExtent.Y;

        if (!isWrapping)
        {
            MinorSize(bounds) = std::max(MinorSize(bounds), Minor(finalSize));
        }

        auto element = m_elementManager.GetAt(rangeIndex);

#ifdef DBG
        const int itemIndexDbg = m_elementManager.GetDataIndexFromRealizedRangeIndex(rangeIndex);

        ITEMSREPEATER_TRACE_VERBOSE(nullptr, TRACE_MSG_METH_IND_STR_STR_INT_FLT_FLT_FLT_FLT, METH_NAME, this,
            winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
            L"Element dataIndex & arrange bounds:",
            itemIndexDbg,
            bounds.X, bounds.Y, bounds.Width, bounds.Height);

        if (itemIndexDbg == m_algorithmCallbacks->Algorithm_GetFlowLayoutLogItemIndexDbg())
        {
            ITEMSREPEATER_TRACE_INFO(nullptr, TRACE_MSG_METH_STR_INT, METH_NAME, this, L"logItemIndexDbg", itemIndexDbg);

            ITEMSREPEATER_TRACE_INFO(nullptr, TRACE_MSG_METH_IND_STR_STR_FLT_FLT_FLT_FLT, METH_NAME, this,
                winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
                L"Element arrange bounds:",
                bounds.X, bounds.Y, bounds.Width, bounds.Height);

            const winrt::ItemsRepeater itemsRepeaterDbg = SharedHelpers::GetAncestorOfType<winrt::ItemsRepeater>(winrt::VisualTreeHelper::GetParent(element));

            if (itemsRepeaterDbg)
            {
                const winrt::UIElement itemsRepeaterParentDbg = winrt::VisualTreeHelper::GetParent(itemsRepeaterDbg).try_as<winrt::UIElement>();

                if (itemsRepeaterParentDbg)
                {
                    const winrt::GeneralTransform transformDbg = element.TransformToVisual(itemsRepeaterParentDbg);
                    const winrt::Point elementOffsetDbg = transformDbg.TransformPoint(winrt::Point{});

                    ITEMSREPEATER_TRACE_INFO(nullptr, TRACE_MSG_METH_IND_STR_STR_FLT_FLT, METH_NAME, this,
                        winrt::get_self<VirtualizingLayoutContext>(m_context.get())->Indent(), layoutId.data(),
                        L"Element arrange offsets within ItemsRepeater parent:",
                        elementOffsetDbg.X, elementOffsetDbg.Y);
                }
            }
        }
#endif // DBG

        element.Arrange(bounds);
    }
}

#pragma endregion

#pragma region Layout Context Helpers

winrt::Rect FlowLayoutAlgorithm::RealizationRect()
{
    return IsVirtualizingContext() ?
        m_context.get().RealizationRect() :
        winrt::Rect{ 0, 0, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
}

void FlowLayoutAlgorithm::SetLayoutOrigin()
{
    if (IsVirtualizingContext())
    {
        m_context.get().LayoutOrigin(winrt::Point{ m_lastExtent.X, m_lastExtent.Y });
    }
    else
    {
        // Should have 0 origin for non-virtualizing layout since we always start from
        // the first item
        MUX_ASSERT(m_lastExtent.X == 0 && m_lastExtent.Y == 0);
    }
}

winrt::UIElement FlowLayoutAlgorithm::GetElementIfRealized(
    int dataIndex)
{
    if (m_elementManager.IsDataIndexRealized(dataIndex))
    {
        return m_elementManager.GetRealizedElement(dataIndex);
    }

    return  nullptr;
}

bool FlowLayoutAlgorithm::IsVirtualizingContext()
{
    if (m_context)
    {
        const auto rect = m_context.get().RealizationRect();
        const bool hasInfiniteSize = (rect.Height == std::numeric_limits<float>::infinity() || rect.Width == std::numeric_limits<float>::infinity());
        return !hasInfiniteSize;
    }
    return false;
}

#pragma endregion
