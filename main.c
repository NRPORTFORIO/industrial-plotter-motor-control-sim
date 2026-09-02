#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// 多品種少量生産や大量生産を見据え、構造体の型定義でもメモリ節約を徹底
// 漫然と int (4バイト) を使わず、1バイト(uint8_t)や2バイト(int16_t)を適切に選択
typedef struct {
    int16_t target_position;  // 目標位置 (プロッタのインクヘッド位置を想定)
    int16_t current_position; // 現在位置
    int16_t target_velocity;  // 目標速度
    int16_t current_velocity; // 現在速度
    uint8_t system_status;     // システム状態 (0: 停止, 1: 稼働, 2: 減速中)
    uint8_t error_code;        // エラーコード (0: 正常, 1: リミット超過, 2: 異常負荷)
    bool is_emergency_active;  // フェイルセーフ（緊急停止）フラグ
} PlotterMotorController;

// プロトタイプ宣言（ポインタ渡しによる高速・省メモリ処理）
void initialize_system(PlotterMotorController *controller);
void update_motor_control(PlotterMotorController *controller, uint8_t limit_sw_left, uint8_t limit_sw_right);
void check_failsafe(PlotterMotorController *controller);
void log_output(const PlotterMotorController *controller, uint8_t step);

int main(void) {
    PlotterMotorController my_plotter;
    initialize_system(&my_plotter);

    // 擬似的な10msリアルタイムループ（10ステップ分の印刷往復運動を模擬）
    // 左右のリミットスイッチ（パルス入力）を引数としてシミュレーション
    printf("=== 産業用プロッタ・モーター制御シミュレーション開始 ===\n");
    printf("Step,TargetPos,CurrentPos,CurrentVel,Status,Emergency\n");

    for (uint8_t step = 1; step <= 10; step++) {
        uint8_t limit_sw_left = 0;
        uint8_t limit_sw_right = 0;

        // ステップ5で右側の端点（リミットスイッチ）に接触したと模擬
        if (step == 5) {
            limit_sw_right = 1;
        }

        // ポインタを渡して制御ロジックを10ms周期で更新
        update_motor_control(&my_plotter, limit_sw_left, limit_sw_right);
        log_output(&my_plotter, step);

        if (my_plotter.is_emergency_active) {
            printf(">> [警告] 緊急停止シグナル検知。安全のためループを遮断します。\n");
            break;
        }
    }

    return 0;
}

// システムの初期化
void initialize_system(PlotterMotorController *controller) {
    controller->target_position = 100;
    controller->current_position = 0;
    controller->target_velocity = 20;
    controller->current_velocity = 0;
    controller->system_status = 1; // 稼働状態
    controller->error_code = 0;
    controller->is_emergency_active = false;
}

// モーターの加減速および追従制御ロジック
void update_motor_control(PlotterMotorController *controller, uint8_t limit_sw_left, uint8_t limit_sw_right) {
    if (controller->is_emergency_active) {
        return; // 緊急停止時は何もしない
    }

    // リミットスイッチを検知した場合、即座にエラーコードを設定してフェイルセーフへ
    if (limit_sw_left == 1 || limit_sw_right == 1) {
        controller->error_code = 1; // リミット超過エラー
        check_failsafe(controller);
        return;
    }

    // 簡易的な台形駆動（位置追従ロジック）
    if (controller->current_position < controller->target_position) {
        controller->current_velocity = controller->target_velocity;
        controller->current_position += controller->current_velocity;
    } else {
        controller->current_velocity = 0;
        controller->system_status = 0; // 目標到達のため停止
    }
}

// フェイルセーフ（安全確保アルゴリズム）の厳格な執行
void check_failsafe(PlotterMotorController *controller) {
    if (controller->error_code != 0) {
        // モーター出力を即座に完全遮断
        controller->current_velocity = 0;
        controller->system_status = 0;
        controller->is_emergency_active = true; // 緊急停止フラグをセット
    }
}

// CSV形式でのログ出力（Pythonツールとの連携を見据えた仕組み化）
void log_output(const PlotterMotorController *controller, uint8_t step) {
    printf("%d,%d,%d,%d,%d,%d\n",
           step,
           controller->target_position,
           controller->current_position,
           controller->current_velocity,
           controller->system_status,
           controller->is_emergency_active ? 1 : 0);
}
