// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_TiltrotorRpcLibServer_hpp
#define air_TiltrotorRpcLibServer_hpp

#include "common/Common.hpp"
#include <functional>
#include "api/RpcLibServerBase.hpp"
#include "vehicles/tiltrotor/api/TiltrotorApiBase.hpp"

namespace msr
{
namespace airlib
{

    class TiltrotorRpcLibServer : public RpcLibServerBase
    {
    public:
        TiltrotorRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port = RpcLibPort);
        virtual ~TiltrotorRpcLibServer();

    protected:
        virtual TiltrotorApiBase* getVehicleApi(const std::string& vehicle_name) override
        {
            return static_cast<TiltrotorApiBase*>(RpcLibServerBase::getVehicleApi(vehicle_name));
        }
    };
}
} //namespace
#endif
