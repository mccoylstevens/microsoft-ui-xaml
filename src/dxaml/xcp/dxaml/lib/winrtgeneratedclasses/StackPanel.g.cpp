// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
//------------------------------------------------------------------------
//
//  Abstract:
//
//      XAML types.
//      NOTE: This file was generated by a tool.
//
//------------------------------------------------------------------------

#include "StackPanel.g.h"
#include "Brush.g.h"
#include "XamlTelemetry.h"

// Constructors/destructors.
DirectUI::StackPanelGenerated::StackPanelGenerated()
{
}

DirectUI::StackPanelGenerated::~StackPanelGenerated()
{
}

HRESULT DirectUI::StackPanelGenerated::QueryInterfaceImpl(_In_ REFIID iid, _Outptr_ void** ppObject)
{
    if (InlineIsEqualGUID(iid, __uuidof(DirectUI::StackPanel)))
    {
        *ppObject = static_cast<DirectUI::StackPanel*>(this);
    }
    else if (InlineIsEqualGUID(iid, __uuidof(ABI::Microsoft::UI::Xaml::Controls::IStackPanel)))
    {
        *ppObject = static_cast<ABI::Microsoft::UI::Xaml::Controls::IStackPanel*>(this);
    }
    else if (InlineIsEqualGUID(iid, __uuidof(ABI::Microsoft::UI::Xaml::Controls::IInsertionPanel)))
    {
        *ppObject = static_cast<ABI::Microsoft::UI::Xaml::Controls::IInsertionPanel*>(this);
    }
    else if (InlineIsEqualGUID(iid, __uuidof(ABI::Microsoft::UI::Xaml::Controls::Primitives::IScrollSnapPointsInfo)))
    {
        *ppObject = static_cast<ABI::Microsoft::UI::Xaml::Controls::Primitives::IScrollSnapPointsInfo*>(this);
    }
    else if (InlineIsEqualGUID(iid, __uuidof(DirectUI::IItemLookupPanel)))
    {
        *ppObject = static_cast<DirectUI::IItemLookupPanel*>(this);
    }
    else if (InlineIsEqualGUID(iid, __uuidof(DirectUI::IPaginatedPanel)))
    {
        *ppObject = static_cast<DirectUI::IPaginatedPanel*>(this);
    }
    else
    {
        RRETURN(DirectUI::Panel::QueryInterfaceImpl(iid, ppObject));
    }

    AddRefOuter();
    RRETURN(S_OK);
}

// Properties.
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_AreHorizontalSnapPointsRegular(_Out_ BOOLEAN* pValue)
{
    HRESULT hr = S_OK;
    ARG_VALIDRETURNPOINTER(pValue);
    *pValue={};
    IFC(CheckThread());
    IFC(static_cast<StackPanel*>(this)->get_AreHorizontalSnapPointsRegularImpl(pValue));
Cleanup:
    RRETURN(hr);
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_AreScrollSnapPointsRegular(_Out_ BOOLEAN* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_AreScrollSnapPointsRegular, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_AreScrollSnapPointsRegular(BOOLEAN value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_AreScrollSnapPointsRegular, value));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_AreVerticalSnapPointsRegular(_Out_ BOOLEAN* pValue)
{
    HRESULT hr = S_OK;
    ARG_VALIDRETURNPOINTER(pValue);
    *pValue={};
    IFC(CheckThread());
    IFC(static_cast<StackPanel*>(this)->get_AreVerticalSnapPointsRegularImpl(pValue));
Cleanup:
    RRETURN(hr);
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_BackgroundSizing(_Out_ ABI::Microsoft::UI::Xaml::Controls::BackgroundSizing* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_BackgroundSizing, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_BackgroundSizing(ABI::Microsoft::UI::Xaml::Controls::BackgroundSizing value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_BackgroundSizing, value));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_BorderBrush(_Outptr_result_maybenull_ ABI::Microsoft::UI::Xaml::Media::IBrush** ppValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_BorderBrush, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_BorderBrush(_In_opt_ ABI::Microsoft::UI::Xaml::Media::IBrush* pValue)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_BorderBrush, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_BorderThickness(_Out_ ABI::Microsoft::UI::Xaml::Thickness* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_BorderThickness, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_BorderThickness(ABI::Microsoft::UI::Xaml::Thickness value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_BorderThickness, value));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_CornerRadius(_Out_ ABI::Microsoft::UI::Xaml::CornerRadius* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_CornerRadius, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_CornerRadius(ABI::Microsoft::UI::Xaml::CornerRadius value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_CornerRadius, value));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_Orientation(_Out_ ABI::Microsoft::UI::Xaml::Controls::Orientation* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_Orientation, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_Orientation(ABI::Microsoft::UI::Xaml::Controls::Orientation value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_Orientation, value));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_Padding(_Out_ ABI::Microsoft::UI::Xaml::Thickness* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_Padding, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_Padding(ABI::Microsoft::UI::Xaml::Thickness value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_Padding, value));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::get_Spacing(_Out_ DOUBLE* pValue)
{
    RRETURN(GetValueByKnownIndex(KnownPropertyIndex::StackPanel_Spacing, pValue));
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::put_Spacing(DOUBLE value)
{
    IFC_RETURN(DefaultStrictApiCheck(this));
    RRETURN(SetValueByKnownIndex(KnownPropertyIndex::StackPanel_Spacing, value));
}

// Events.
_Check_return_ HRESULT DirectUI::StackPanelGenerated::GetHorizontalSnapPointsChangedEventSourceNoRef(_Outptr_ HorizontalSnapPointsChangedEventSourceType** ppEventSource)
{
    HRESULT hr = S_OK;

    IFC(GetEventSourceNoRefWithArgumentValidation(KnownEventIndex::StackPanel_HorizontalSnapPointsChanged, reinterpret_cast<IUntypedEventSource**>(ppEventSource)));

    if (*ppEventSource == nullptr)
    {
        IFC(ctl::ComObject<HorizontalSnapPointsChangedEventSourceType>::CreateInstance(ppEventSource, TRUE /* fDisableLeakChecks */));
        (*ppEventSource)->Initialize(KnownEventIndex::StackPanel_HorizontalSnapPointsChanged, this, /* bUseEventManager */ false);
        IFC(StoreEventSource(KnownEventIndex::StackPanel_HorizontalSnapPointsChanged, *ppEventSource));

        // The caller doesn't expect a ref, this function ends in "NoRef".  The ref is now owned by the map (inside StoreEventSource)
        ReleaseInterfaceNoNULL(ctl::iunknown_cast(*ppEventSource));
    }

Cleanup:
    return hr;
}

IFACEMETHODIMP DirectUI::StackPanelGenerated::add_HorizontalSnapPointsChanged(_In_ ABI::Windows::Foundation::IEventHandler<IInspectable*>* pValue, _Out_ EventRegistrationToken* ptToken)
{
    HRESULT hr = S_OK;
    HorizontalSnapPointsChangedEventSourceType* pEventSource = nullptr;

    IFC(EventAddPreValidation(pValue, ptToken));
    IFC(DefaultStrictApiCheck(this));
    IFC(GetHorizontalSnapPointsChangedEventSourceNoRef(&pEventSource));
    IFC(pEventSource->AddHandler(pValue));

    ptToken->value = (INT64)pValue;

Cleanup:
    return hr;
}

IFACEMETHODIMP DirectUI::StackPanelGenerated::remove_HorizontalSnapPointsChanged(EventRegistrationToken tToken)
{
    HRESULT hr = S_OK;
    HorizontalSnapPointsChangedEventSourceType* pEventSource = nullptr;
    ABI::Windows::Foundation::IEventHandler<IInspectable*>* pValue = (ABI::Windows::Foundation::IEventHandler<IInspectable*>*)tToken.value;

    IFC(CheckThread());
    IFC(DefaultStrictApiCheck(this));
    IFC(GetHorizontalSnapPointsChangedEventSourceNoRef(&pEventSource));
    IFC(pEventSource->RemoveHandler(pValue));

    if (!pEventSource->HasHandlers())
    {
        IFC(RemoveEventSource(KnownEventIndex::StackPanel_HorizontalSnapPointsChanged));
    }

Cleanup:
    RRETURN(hr);
}
_Check_return_ HRESULT DirectUI::StackPanelGenerated::GetVerticalSnapPointsChangedEventSourceNoRef(_Outptr_ VerticalSnapPointsChangedEventSourceType** ppEventSource)
{
    HRESULT hr = S_OK;

    IFC(GetEventSourceNoRefWithArgumentValidation(KnownEventIndex::StackPanel_VerticalSnapPointsChanged, reinterpret_cast<IUntypedEventSource**>(ppEventSource)));

    if (*ppEventSource == nullptr)
    {
        IFC(ctl::ComObject<VerticalSnapPointsChangedEventSourceType>::CreateInstance(ppEventSource, TRUE /* fDisableLeakChecks */));
        (*ppEventSource)->Initialize(KnownEventIndex::StackPanel_VerticalSnapPointsChanged, this, /* bUseEventManager */ false);
        IFC(StoreEventSource(KnownEventIndex::StackPanel_VerticalSnapPointsChanged, *ppEventSource));

        // The caller doesn't expect a ref, this function ends in "NoRef".  The ref is now owned by the map (inside StoreEventSource)
        ReleaseInterfaceNoNULL(ctl::iunknown_cast(*ppEventSource));
    }

Cleanup:
    return hr;
}

IFACEMETHODIMP DirectUI::StackPanelGenerated::add_VerticalSnapPointsChanged(_In_ ABI::Windows::Foundation::IEventHandler<IInspectable*>* pValue, _Out_ EventRegistrationToken* ptToken)
{
    HRESULT hr = S_OK;
    VerticalSnapPointsChangedEventSourceType* pEventSource = nullptr;

    IFC(EventAddPreValidation(pValue, ptToken));
    IFC(DefaultStrictApiCheck(this));
    IFC(GetVerticalSnapPointsChangedEventSourceNoRef(&pEventSource));
    IFC(pEventSource->AddHandler(pValue));

    ptToken->value = (INT64)pValue;

Cleanup:
    return hr;
}

IFACEMETHODIMP DirectUI::StackPanelGenerated::remove_VerticalSnapPointsChanged(EventRegistrationToken tToken)
{
    HRESULT hr = S_OK;
    VerticalSnapPointsChangedEventSourceType* pEventSource = nullptr;
    ABI::Windows::Foundation::IEventHandler<IInspectable*>* pValue = (ABI::Windows::Foundation::IEventHandler<IInspectable*>*)tToken.value;

    IFC(CheckThread());
    IFC(DefaultStrictApiCheck(this));
    IFC(GetVerticalSnapPointsChangedEventSourceNoRef(&pEventSource));
    IFC(pEventSource->RemoveHandler(pValue));

    if (!pEventSource->HasHandlers())
    {
        IFC(RemoveEventSource(KnownEventIndex::StackPanel_VerticalSnapPointsChanged));
    }

Cleanup:
    RRETURN(hr);
}

// Methods.
IFACEMETHODIMP DirectUI::StackPanelGenerated::GetInsertionIndexes(ABI::Windows::Foundation::Point position, _Out_ INT* pFirst, _Out_ INT* pSecond)
{
    HRESULT hr = S_OK;
    if (EventEnabledApiFunctionCallStart())
    {
        XamlTelemetry::PublicApiCall(true, reinterpret_cast<uint64_t>(this), "StackPanel_GetInsertionIndexes", 0);
    }
    
    IFC(CheckThread());
    IFC(DefaultStrictApiCheck(this));
    IFC(static_cast<StackPanel*>(this)->GetInsertionIndexesImpl(position, pFirst, pSecond));
Cleanup:
    if (EventEnabledApiFunctionCallStop())
    {
        XamlTelemetry::PublicApiCall(false, reinterpret_cast<uint64_t>(this), "StackPanel_GetInsertionIndexes", hr);
    }
    RRETURN(hr);
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::GetIrregularSnapPoints(ABI::Microsoft::UI::Xaml::Controls::Orientation orientation, ABI::Microsoft::UI::Xaml::Controls::Primitives::SnapPointsAlignment alignment, _Outptr_ ABI::Windows::Foundation::Collections::IVectorView<FLOAT>** ppReturnValue)
{
    HRESULT hr = S_OK;
    if (EventEnabledApiFunctionCallStart())
    {
        XamlTelemetry::PublicApiCall(true, reinterpret_cast<uint64_t>(this), "StackPanel_GetIrregularSnapPoints", 0);
    }
    ARG_VALIDRETURNPOINTER(ppReturnValue);
    *ppReturnValue={};
    IFC(CheckThread());
    IFC(DefaultStrictApiCheck(this));
    IFC(static_cast<StackPanel*>(this)->GetIrregularSnapPointsImpl(orientation, alignment, ppReturnValue));
Cleanup:
    if (EventEnabledApiFunctionCallStop())
    {
        XamlTelemetry::PublicApiCall(false, reinterpret_cast<uint64_t>(this), "StackPanel_GetIrregularSnapPoints", hr);
    }
    RRETURN(hr);
}
IFACEMETHODIMP DirectUI::StackPanelGenerated::GetRegularSnapPoints(ABI::Microsoft::UI::Xaml::Controls::Orientation orientation, ABI::Microsoft::UI::Xaml::Controls::Primitives::SnapPointsAlignment alignment, _Out_ FLOAT* pOffset, _Out_ FLOAT* pReturnValue)
{
    HRESULT hr = S_OK;
    if (EventEnabledApiFunctionCallStart())
    {
        XamlTelemetry::PublicApiCall(true, reinterpret_cast<uint64_t>(this), "StackPanel_GetRegularSnapPoints", 0);
    }
    ARG_VALIDRETURNPOINTER(pReturnValue);
    *pReturnValue={};
    IFC(CheckThread());
    IFC(DefaultStrictApiCheck(this));
    IFC(static_cast<StackPanel*>(this)->GetRegularSnapPointsImpl(orientation, alignment, pOffset, pReturnValue));
Cleanup:
    if (EventEnabledApiFunctionCallStop())
    {
        XamlTelemetry::PublicApiCall(false, reinterpret_cast<uint64_t>(this), "StackPanel_GetRegularSnapPoints", hr);
    }
    RRETURN(hr);
}

_Check_return_ HRESULT DirectUI::StackPanelGenerated::EventAddHandlerByIndex(_In_ KnownEventIndex nEventIndex, _In_ IInspectable* pHandler, _In_ BOOLEAN handledEventsToo)
{
    switch (nEventIndex)
    {
    case KnownEventIndex::StackPanel_HorizontalSnapPointsChanged:
        {
            ctl::ComPtr<ABI::Windows::Foundation::IEventHandler<IInspectable*>> spEventHandler;
            IFC_RETURN(IValueBoxer::UnboxValue(pHandler, spEventHandler.ReleaseAndGetAddressOf()));

            if (nullptr != spEventHandler)
            {
                HorizontalSnapPointsChangedEventSourceType* pEventSource = nullptr;
                IFC_RETURN(GetHorizontalSnapPointsChangedEventSourceNoRef(&pEventSource));
                IFC_RETURN(pEventSource->AddHandler(spEventHandler.Get()));
            }
            else
            {
                IFC_RETURN(E_INVALIDARG);
            }
        }
        break;
    case KnownEventIndex::StackPanel_VerticalSnapPointsChanged:
        {
            ctl::ComPtr<ABI::Windows::Foundation::IEventHandler<IInspectable*>> spEventHandler;
            IFC_RETURN(IValueBoxer::UnboxValue(pHandler, spEventHandler.ReleaseAndGetAddressOf()));

            if (nullptr != spEventHandler)
            {
                VerticalSnapPointsChangedEventSourceType* pEventSource = nullptr;
                IFC_RETURN(GetVerticalSnapPointsChangedEventSourceNoRef(&pEventSource));
                IFC_RETURN(pEventSource->AddHandler(spEventHandler.Get()));
            }
            else
            {
                IFC_RETURN(E_INVALIDARG);
            }
        }
        break;
    default:
        IFC_RETURN(DirectUI::PanelGenerated::EventAddHandlerByIndex(nEventIndex, pHandler, handledEventsToo));
        break;
    }

    return S_OK;
}

_Check_return_ HRESULT DirectUI::StackPanelGenerated::EventRemoveHandlerByIndex(_In_ KnownEventIndex nEventIndex, _In_ IInspectable* pHandler)
{
    switch (nEventIndex)
    {
    case KnownEventIndex::StackPanel_HorizontalSnapPointsChanged:
        {
            ctl::ComPtr<ABI::Windows::Foundation::IEventHandler<IInspectable*>> spEventHandler;
            IFC_RETURN(IValueBoxer::UnboxValue(pHandler, spEventHandler.ReleaseAndGetAddressOf()));

            if (nullptr != spEventHandler)
            {
                HorizontalSnapPointsChangedEventSourceType* pEventSource = nullptr;
                IFC_RETURN(GetHorizontalSnapPointsChangedEventSourceNoRef(&pEventSource));
                IFC_RETURN(pEventSource->RemoveHandler(spEventHandler.Get()));
            }
            else
            {
                IFC_RETURN(E_INVALIDARG);
            }
        }
        break;
    case KnownEventIndex::StackPanel_VerticalSnapPointsChanged:
        {
            ctl::ComPtr<ABI::Windows::Foundation::IEventHandler<IInspectable*>> spEventHandler;
            IFC_RETURN(IValueBoxer::UnboxValue(pHandler, spEventHandler.ReleaseAndGetAddressOf()));

            if (nullptr != spEventHandler)
            {
                VerticalSnapPointsChangedEventSourceType* pEventSource = nullptr;
                IFC_RETURN(GetVerticalSnapPointsChangedEventSourceNoRef(&pEventSource));
                IFC_RETURN(pEventSource->RemoveHandler(spEventHandler.Get()));
            }
            else
            {
                IFC_RETURN(E_INVALIDARG);
            }
        }
        break;
    default:
        IFC_RETURN(DirectUI::PanelGenerated::EventRemoveHandlerByIndex(nEventIndex, pHandler));
        break;
    }

    return S_OK;
}

HRESULT DirectUI::StackPanelFactory::QueryInterfaceImpl(_In_ REFIID iid, _Outptr_ void** ppObject)
{
    if (InlineIsEqualGUID(iid, __uuidof(ABI::Microsoft::UI::Xaml::Controls::IStackPanelFactory)))
    {
        *ppObject = static_cast<ABI::Microsoft::UI::Xaml::Controls::IStackPanelFactory*>(this);
    }
    else if (InlineIsEqualGUID(iid, __uuidof(ABI::Microsoft::UI::Xaml::Controls::IStackPanelStatics)))
    {
        *ppObject = static_cast<ABI::Microsoft::UI::Xaml::Controls::IStackPanelStatics*>(this);
    }
    else
    {
        RRETURN(ctl::BetterAggregableCoreObjectActivationFactory::QueryInterfaceImpl(iid, ppObject));
    }

    AddRefOuter();
    RRETURN(S_OK);
}


// Factory methods.
IFACEMETHODIMP DirectUI::StackPanelFactory::CreateInstance(_In_opt_ IInspectable* pOuter, _Outptr_ IInspectable** ppInner, _Outptr_ ABI::Microsoft::UI::Xaml::Controls::IStackPanel** ppInstance)
{

#if DBG
    // We play some games with reinterpret_cast and assuming that the GUID type table is accurate - which is somewhat sketchy, but
    // really good for binary size.  This code is a sanity check that the games we play are ok.
    const GUID uuidofGUID = __uuidof(ABI::Microsoft::UI::Xaml::Controls::IStackPanel);
    const GUID metadataAPIGUID = MetadataAPI::GetClassInfoByIndex(GetTypeIndex())->GetGuid();
    const KnownTypeIndex typeIndex = GetTypeIndex();

    if(uuidofGUID != metadataAPIGUID)
    {
        XAML_FAIL_FAST();
    }
#endif

    // Can't just IFC(_RETURN) this because for some validate calls (those with multiple template parameters), the
    // preprocessor gets confused at the "," in the template type-list before the function's opening parenthesis.
    // So we'll use IFC_RETURN syntax with a local hr variable, kind of weirdly.
    const HRESULT hr = ctl::ValidateFactoryCreateInstanceWithBetterAggregableCoreObjectActivationFactory(pOuter, ppInner, reinterpret_cast<IUnknown**>(ppInstance), GetTypeIndex(), false /*isFreeThreaded*/);
    IFC_RETURN(hr);
    return S_OK;
}

// Dependency properties.
IFACEMETHODIMP DirectUI::StackPanelFactory::get_AreScrollSnapPointsRegularProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_AreScrollSnapPointsRegular, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_OrientationProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_Orientation, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_BackgroundSizingProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_BackgroundSizing, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_BorderBrushProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_BorderBrush, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_BorderThicknessProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_BorderThickness, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_CornerRadiusProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_CornerRadius, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_PaddingProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_Padding, ppValue));
}
IFACEMETHODIMP DirectUI::StackPanelFactory::get_SpacingProperty(_Out_ ABI::Microsoft::UI::Xaml::IDependencyProperty** ppValue)
{
    RRETURN(MetadataAPI::GetIDependencyProperty(KnownPropertyIndex::StackPanel_Spacing, ppValue));
}

// Attached properties.

// Static properties.

// Static methods.

namespace DirectUI
{
    _Check_return_ IActivationFactory* CreateActivationFactory_StackPanel()
    {
        RRETURN(ctl::ActivationFactoryCreator<StackPanelFactory>::CreateActivationFactory());
    }
}
