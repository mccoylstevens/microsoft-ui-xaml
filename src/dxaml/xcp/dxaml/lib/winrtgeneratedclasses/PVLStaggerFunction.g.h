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

#pragma once

#include "StaggerFunctionBase.g.h"

#define __PVLStaggerFunction_GUID "43f8ee08-3932-4ee4-b391-398eebd5eb89"

namespace DirectUI
{
    class PVLStaggerFunction;

    class __declspec(novtable) PVLStaggerFunctionGenerated:
        public DirectUI::StaggerFunctionBase
    {
        friend class DirectUI::PVLStaggerFunction;



    public:
        PVLStaggerFunctionGenerated();
        ~PVLStaggerFunctionGenerated() override;

        // Event source typedefs.

        KnownTypeIndex GetTypeIndex() const override
        {
            return KnownTypeIndex::PVLStaggerFunction;
        }

        static XCP_FORCEINLINE KnownTypeIndex GetTypeIndexStatic()
        {
            return KnownTypeIndex::PVLStaggerFunction;
        }

        // Properties.
        _Check_return_ HRESULT get_Delay(_Out_ DOUBLE* pValue);
        _Check_return_ HRESULT put_Delay(DOUBLE value);
        _Check_return_ HRESULT get_DelayReduce(_Out_ DOUBLE* pValue);
        _Check_return_ HRESULT put_DelayReduce(DOUBLE value);
        _Check_return_ HRESULT get_Maximum(_Out_ DOUBLE* pValue);
        _Check_return_ HRESULT put_Maximum(DOUBLE value);
        _Check_return_ HRESULT get_Reverse(_Out_ BOOLEAN* pValue);
        _Check_return_ HRESULT put_Reverse(BOOLEAN value);

        // Events.

        // Methods.


    protected:
        HRESULT QueryInterfaceImpl(_In_ REFIID iid, _Outptr_ void** ppObject) override;

    private:

        // Fields.
    };
}

#include "PVLStaggerFunction_Partial.h"

