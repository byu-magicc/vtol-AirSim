#ifndef msr_airlib_vehicles_VtolSimple_hpp
#define msr_airlib_vehicles_VtolSimple_hpp

#include "vehicles/vtol/firmwares/vtol_simple/VtolSimpleApi.hpp"
#include "vehicles/vtol/AeroBodyParams.hpp"
#include "common/AirSimSettings.hpp"
#include "sensors/SensorFactory.hpp"

namespace msr
{
namespace airlib
{

    class VtolSimpleParams : public AeroBodyParams
    {
    public:
        VtolSimpleParams(const AirSimSettings::VehicleSetting* vehicle_setting, std::shared_ptr<const SensorFactory> sensor_factory)
            : vehicle_setting_(vehicle_setting), sensor_factory_(sensor_factory)
        {
        }

        virtual ~VtolSimpleParams() = default;

        virtual std::unique_ptr<VtolApiBase> createVtolApi() override
        {
            return std::unique_ptr<VtolApiBase>(new VtolSimpleApi(this, vehicle_setting_));
        }

    protected:
        virtual void setupParams() override
        {
            auto& params = getParams();
            setupTriTiltrotor(params);
        }

        virtual const SensorFactory* getSensorFactory() const override
        {
            return sensor_factory_.get();
        }

    private:
        vector<unique_ptr<SensorBase>> sensor_storage_;
        const AirSimSettings::VehicleSetting* vehicle_setting_; //store as pointer because of derived classes
        std::shared_ptr<const SensorFactory> sensor_factory_;
    };

}
} //namespace
#endif
