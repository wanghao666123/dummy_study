
#include "common_inc.h"

/*----------------- 1.Add Your Extern Variables Here (Optional) ------------------*/
extern DummyRobot dummy;
#define TEST_ADC_CHANNEL_0 0
class HelperFunctions
{
public:
    /*--------------- 2.Add Your Helper Functions Helper Here (optional) ----------------*/
    float GetTemperatureHelper()
    {
        return AdcGetChipTemperature();
    }

    /*just AN test for how to add a new CLI command through fibre protocol*/
    float GetVoltageHelper()
    {
        return AdcGetVoltage(TEST_ADC_CHANNEL_0);
    }


} staticFunctions;
//!允许在程序的其他部分通过 staticFunctions 直接访问 HelperFunctions 的成员函数，而无需额外创建实例
// Define options that intractable with "reftool".
static inline auto MakeObjTree()
{
    /*--------------- 3.Add Your Protocol Variables & Functions Here ----------------*/
    return make_protocol_member_list(
        // Add Read-Only Variables
        //!定义一个只读属性，返回值是ProtocolProperty
        make_protocol_ro_property("serial_number", &serialNumber),
        //!定义一个协议函数，返回值是ProtocolFunction
        make_protocol_function("get_temperature", staticFunctions, &HelperFunctions::GetTemperatureHelper),
        //!定义一个协议函数，返回值是ProtocolFunction
        make_protocol_function("get_voltage", staticFunctions, &HelperFunctions::GetVoltageHelper),
        //!定义一个嵌套的协议对象
        make_protocol_object("robot", dummy.MakeProtocolDefinitions())
    );
}


COMMIT_PROTOCOL
