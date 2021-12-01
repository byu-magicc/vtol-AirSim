// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_VtolRpcLibServer_hpp
#define air_VtolRpcLibServer_hpp

#include "common/Common.hpp"
#include <functional>
#include "api/RpcLibServerBase.hpp"
#include "vehicles/vtol/api/VtolApiBase.hpp"

namespace msr
{
namespace airlib
{

    class VtolRpcLibServer : public RpcLibServerBase
    {
    public:
        VtolRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port = RpcLibPort);
        virtual ~VtolRpcLibServer();

    protected:
        virtual VtolApiBase* getVehicleApi(const std::string& vehicle_name) override
        {
            return static_cast<VtolApiBase*>(RpcLibServerBase::getVehicleApi(vehicle_name));
        }
    };
}
} //namespace
#endif
