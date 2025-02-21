#ifndef DOF6_KINEMATIC_SOLVER_H
#define DOF6_KINEMATIC_SOLVER_H

#include "stm32f405xx.h"
#include "arm_math.h"
#include "memory.h"

class DOF6Kinematic
{
private:
    //!将 弧度 转换为 度
    const float RAD_TO_DEG = 57.295777754771045f;

    // DH parameters
    struct ArmConfig_t
    {
        float L_BASE;//!机器人的基座长度
        float D_BASE;//!基座的偏移量
        float L_ARM;//!机器臂的长度
        float L_FOREARM;//!前臂的长度
        float D_ELBOW;//!肘部的偏移量
        float L_WRIST;//!手腕的长度
    };
    ArmConfig_t armConfig;
    //!DH_matrix 是一个 6x4 的矩阵，用于存储机器人的 DH 参数，即 Denavit-Hartenberg 参数。每一行代表一个关节的四个参数：

    //!home：该关节的零位（参考位）。
    //!d：该关节的偏移量。
    //!a：该关节的连杆长度。
    //!alpha：该关节的扭转角度。

    float DH_matrix[6][4] = {0}; // home,d,a,alpha
    //!机器人不同部件的 3D 坐标（x, y, z）。每个数组表示一个坐标位置
    //!L1_base：基座的位置
    //!L2_arm：臂部的末端位置
    //!L3_elbow：肘部的位置
    //!L6_wrist：手腕的位置
    float L1_base[3] = {0};
    float L2_arm[3] = {0};
    float L3_elbow[3] = {0};
    float L6_wrist[3] = {0};

    //!l_se_2 和 l_se：可能是某个连杆的平方和长度，用于描述臂长
    //!l_ew_2 和 l_ew：与肘部相关的长度计算
    //!atan_e：计算肘部角度
    float l_se_2;
    float l_se;
    float l_ew_2;
    float l_ew;
    float atan_e;

public:
    struct Joint6D_t
    {
        Joint6D_t()
        = default;

        Joint6D_t(float a1, float a2, float a3, float a4, float a5, float a6)
            : a{a1, a2, a3, a4, a5, a6}
        {}

        float a[6];

        friend Joint6D_t operator-(const Joint6D_t &_joints1, const Joint6D_t &_joints2);
    };

    struct Pose6D_t
    {
        Pose6D_t()
        = default;
        //!位置（X, Y, Z）和旋转（A, B, C）
        //!X, Y, Z: 位置坐标，表示机器人或物体在三维空间中的位置
        //!A, B, C: 旋转角度，可能表示物体绕 XYZ 轴的旋转角度（通常是欧拉角）
        Pose6D_t(float x, float y, float z, float a, float b, float c)
            : X(x), Y(y), Z(z), A(a), B(b), C(c), hasR(false)
        {}

        float X{}, Y{}, Z{};
        float A{}, B{}, C{};
        float R[9]{};

        // if Pose was calculated by FK then it's true automatically (so that no need to do extra calc),
        // otherwise if manually set params then it should be set to false.
        //!FK（Forward Kinematics） 是指通过已知的关节角度计算机器人末端的位置和方向。如果 Pose 是通过 FK 计算得出的，那么它的旋转信息是自动计算的，不需要额外赋值，因此 hasR = true。
        //!手动设定参数（例如直接给定目标位置和方向）时，hasR 应该是 false，表明程序需要手动处理旋转信息。
        bool hasR{};
    };

    struct IKSolves_t
    {
        Joint6D_t config[8];
        char solFlag[8][3];
    };

    DOF6Kinematic(float L_BS, float D_BS, float L_AM, float L_FA, float D_EW, float L_WT);

    bool SolveFK(const Joint6D_t &_inputJoint6D, Pose6D_t &_outputPose6D);

    bool SolveIK(const Pose6D_t &_inputPose6D, const Joint6D_t &_lastJoint6D, IKSolves_t &_outputSolves);
};

#endif //DOF6_KINEMATIC_SOLVER_H
