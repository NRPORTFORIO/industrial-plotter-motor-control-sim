import numpy as np
import matplotlib.pyplot as plt

def analyze_plotter_logs():
    print("=== 産業用プロッタ・テストデータ自動解析開始 ===")
    
    # 1. C言語シミュレータが吐き出すCSV形式のログデータを模擬的に定義
    # 形式: [Step, TargetPos, CurrentPos, CurrentVel, Status, Emergency]
    log_data = np.array([,
 ,
 ,
 ,
 , # ステップ5でリミットスイッチ接触、緊急停止発動
    ])
    
    steps = log_data[:, 0]
    target_pos = log_data[:, 1]
    current_pos = log_data[:, 2]
    emergency_flag = log_data[:, 5]
    
    # 2. 客観的な評価：目標位置と現在位置の「追従エラー（ズレ）」を自動計算
    # 評価工数を削減し、製品の正確性をデータから定量的に判定するロジック
    tracking_errors = target_pos - current_pos
    
    print("\n[解析結果レポート]")
    for i, error in enumerate(tracking_errors):
        print(f"ステップ {int(steps[i])}: 追従エラー = {error} mm")
        if emergency_flag[i] == 1:
            print(f"  -> ⚠️【警告】ステップ {int(steps[i])} にてEMERGENCY_STOP（安全回路）の作動を検知。制御を強制終了しました。")
            break

    # 3. グラフ化による仕組み化（実務での検査・評価の視覚化を想定）
    # ※GitHub上に可視化のコードを載せておくことで、ツールの作成能力を証明します。
    plt.figure(figsize=(10, 5))
    plt.plot(steps[:len(tracking_errors)], target_pos[:len(tracking_errors)], label="Target Position (mm)", marker='o', linestyle='--')
    plt.plot(steps[:len(tracking_errors)], current_pos[:len(tracking_errors)], label="Current Position (mm)", marker='s')
    plt.bar(steps[:len(tracking_errors)], tracking_errors, alpha=0.3, color='red', label="Tracking Error (mm)")
    
    plt.title("Industrial Plotter Motor Control Analysis")
    plt.xlabel("Simulation Step (10ms interval)")
    plt.ylabel("Position / Error")
    plt.grid(True)
    plt.legend()
    
    # 実務でのレポート自動生成を想定し、解析結果を画像として保存する設計
    plt.savefig("motor_control_analysis.png")
    print("\n>> グラフ解析結果を 'motor_control_analysis.png' として自動出力しました。")
    print("=== 解析処理完了 ===")

if __name__ == "__main__":
    analyze_plotter_logs()
