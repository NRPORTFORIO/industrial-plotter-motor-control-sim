def analyze_plotter_logs():
    print("=== 産業用プロッタ・テストデータ自動解析開始 ===")
    
    # 1. C言語シミュレータが吐き出すCSV形式のログデータを模擬的に定義
    # 形式: [Step, TargetPos, CurrentPos, CurrentVel, Status, Emergency]
    log_data = [
        [0,  0.0,  0.0, 0.0, 0, 0],
        [1, 10.0,  9.5, 9.5, 0, 0],
        [2, 20.0, 19.0, 9.5, 0, 0],
        [3, 30.0, 28.2, 9.2, 0, 0],
        [4, 40.0, 37.5, 9.3, 0, 0],
        [5, 50.0, 41.0, 3.5, 1, 1]  # ステップ5で緊急停止発動
    ]
    
    print("\n[解析結果レポート]")
    
    # ループ処理でデータを1行ずつ解析（インデントを4マス刻みで統一）
    for row in log_data:
        step = int(row[0])
        target_pos = row[1]
        current_pos = row[2]
        emergency_flag = row[5]

        # 追従エラー（ズレ）の自動計算
        error = round(target_pos - current_pos, 1)

        print(f"ステップ {step}: 追従エラー = {error} mm")

        if emergency_flag == 1:
            print(f"  -> ⚠️【警告】ステップ {step} にてEMERGENCY_STOP（安全回路）の作動を検知。制御を強制終了しました。")
            break

    print("\n=== 解析処理完了 ===")

if __name__ == "__main__":
    analyze_plotter_logs()
