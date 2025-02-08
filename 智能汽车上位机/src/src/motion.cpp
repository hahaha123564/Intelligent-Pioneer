/**
 ********************************************************************************************************
 *                                               示例代码
 *                                             EXAMPLE  CODE
 *
 *                      (c) Copyright 2024; SaiShu.Lcc.; Leo; https://bjsstech.com
 *                                   版权所属[SASU-北京赛曙科技有限公司]
 *
 *            The code is for internal use only, not for commercial transactions(开源学习,请勿商用).
 *            The code ADAPTS the corresponding hardware circuit board(代码适配百度Edgeboard-智能汽车赛事版),
 *            The specific details consult the professional(欢迎联系我们,代码持续更正，敬请关注相关开源渠道).
 *********************************************************************************************************
 * @file motion.cpp
 * @author Leo
 * @brief 运动控制器：PD姿态控制||速度控制
 * @version 0.1
 * @date 2023-12-26
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <fstream>
#include <iostream>
#include <cmath>
#include "../include/common.hpp"
#include "../include/json.hpp"
#include "controlcenter.cpp"

using namespace std;
using namespace cv;

/**
 * @brief 运动控制器
 *
 */
class Motion
{
private:
    int countShift = 0; // 变速计数器

public:
    /**
     * @brief 初始化：加载配置文件
     *
     */
    Motion()
    {
        string jsonPath = "../src/config/config.json";
        std::ifstream config_is(jsonPath);
        if (!config_is.good())
        {
            std::cout << "Error: Params file path:[" << jsonPath
                      << "] not find .\n";
            exit(-1);
        }

        nlohmann::json js_value;
        config_is >> js_value;

        try
        {
            params = js_value.get<Params>();
        }
        catch (const nlohmann::detail::exception &e)
        {
            std::cerr << "Json Params Parse failed :" << e.what() << '\n';
            exit(-1);
        }

        speed = params.speedLow;
        cout << "--- runP1:" << params.runP1 << " | runP2:" << params.runP2 << " | runP3:" << params.runP3 << endl;
        cout << "--- turnP:" << params.turnP << " | turnD:" << params.turnD << endl;
        cout << "--- speedLow:" << params.speedLow << "m/s  |  speedHigh:" << params.speedHigh << "m/s" << endl;
    };

    /**
     * @brief 控制器核心参数
     *
     */
    struct Params
    {
        float speedLow = 0.8;                              // 智能车最低速
        float speedHigh = 0.8;                             // 智能车最高速
        float speedBridge = 0.6;                           // 坡道速度
        float speedDown = 0.5;                             // 特殊区域降速速度
        float runP1 = 0.9;                                 // 一阶比例系数：直线控制量
        float runP2 = 0.018;                               // 二阶比例系数：弯道控制量
        float runP3 = 0.0;                                 // 三阶比例系数：弯道控制量
        float turnP = 3.5;                                 // 一阶比例系数：转弯控制量
        float turnD = 3.5;                                 // 一阶微分系数：转弯控制量
        bool debug = false;                                // 调试模式使能
        bool saveImg = false;                              // 存图使能
        uint16_t rowCutUp = 10;                            // 图像顶部切行
        uint16_t rowCutBottom = 10;                        // 图像顶部切行
        bool bridge = true;                                // 坡道区使能
        bool danger = true;                                // 危险区使能
        bool rescue = true;                                // 救援区使能
        bool racing = true;                                // 追逐区使能
        bool parking = true;                               // 停车区使能
        bool ring = true;                                  // 环岛使能
        bool cross = true;                                 // 十字道路使能
        float score = 0.5;                                 // AI检测置信度
        string model = "../res/model/yolov3_mobilenet_v1"; // 模型路径
        string video = "../res/samples/demo.mp4";          // 视频路径
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Params, speedLow, speedHigh, speedBridge, speedDown, runP1, runP2, runP3,
                                       turnP, turnD, debug, saveImg, rowCutUp, rowCutBottom, bridge, danger,
                                       rescue, racing, parking, ring, cross, score, model, video); // 添加构造函数
    };

    Params params;                   // 读取控制参数
    uint16_t servoPwm = PWMSERVOMID; // 发送给舵机的PWM
    float speed = 0.3;               // 发送给电机的速度
    /**
     * @brief 姿态PD控制器
     *
     * @param controlCenter 智能车控制中心
     */
    void poseCtrl(int controlCenter)
    {
        float error = controlCenter - COLSIMAGE / 2; // 图像控制中心转换偏差
        static int errorLast = 0;                    // 记录前一次的偏差
        if (abs(error - errorLast) > COLSIMAGE / 10)
        {
            error = error > errorLast ? errorLast + COLSIMAGE / 10 : errorLast - COLSIMAGE / 10;
        }

        params.turnP = abs(error) * params.runP2 + params.runP1;
        int pwmDiff = (error * params.turnP) + (error - errorLast) * params.turnD;
        errorLast = error;

        servoPwm = (uint16_t)(PWMSERVOMID + pwmDiff); // PWM转换
    }

    /**
     * @brief 变加速控制
     *
     * @param enable 加速使能
     * @param control
     */
    void speedCtrl(bool enable, bool slowDown, ControlCenter control)
    {
        // 控制率
        uint8_t controlLow = 0;   // 速度控制下限
        uint8_t controlMid = 5;   // 控制率
        uint8_t controlHigh = 10; // 速度控制上限

        if (slowDown)
        {
            countShift = controlLow;
            speed = params.speedDown;
        }
        else if (enable) // 加速使能
        {
            if (control.centerEdge.size() < 10)
            {
                speed = params.speedLow;
                countShift = controlLow;
                return;
            }
            if (control.centerEdge[control.centerEdge.size() - 1].x > ROWSIMAGE / 2)
            {
                speed = params.speedLow;
                countShift = controlLow;
                return;
            }
            if (abs(control.sigmaCenter) < 100.0)
            {
                countShift++;
                if (countShift > controlHigh)
                    countShift = controlHigh;
            }
            else
            {
                countShift--;
                if (countShift < controlLow)
                    countShift = controlLow;
            }

            if (countShift > controlMid)
                speed = params.speedHigh;
            else
                speed = params.speedLow;
        }
        else
        {
            countShift = controlLow;
            speed = params.speedLow;
        }
    }
};
