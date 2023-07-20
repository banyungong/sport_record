//
//  ViewController.swift
//  TestProject
//
//  Created by Kerwin on 2023/6/28.
//

import UIKit
import SnapKit
import PreventionCheatKit

class ViewController: UIViewController {
    
    var timeObserve: DispatchSourceTimer! //定时任务

    override func viewDidLoad() {
        super.viewDidLoad()
        view.backgroundColor = .brown
        
        let btn = UIButton(type: .custom)
        view.addSubview(btn)
        btn.snp.makeConstraints { make in
            make.left.equalTo(10)
            make.top.equalTo(100)
            make.width.equalTo(100)
            make.height.equalTo(44)
        }
        btn.setTitle("start", for: .normal)
        btn.setTitleColor(.blue, for: .normal)
        btn.addTarget(self, action: #selector(start), for: .touchUpInside)
        

        let btn2 = UIButton(type: .custom)
        btn2.setTitle("filter", for: .normal)
        btn2.setTitleColor(.blue, for: .normal)
        btn2.addTarget(self, action: #selector(filter), for: .touchUpInside)
        view.addSubview(btn2)
        btn2.snp.makeConstraints { make in
            make.left.equalTo(btn.snp.right).offset(20)
            make.top.equalTo(btn)
            make.width.equalTo(100)
            make.height.equalTo(44)
        }

        let btn3 = UIButton(type: .custom)
        btn3.setTitle("clean", for: .normal)
        btn3.setTitleColor(.blue, for: .normal)
        btn3.addTarget(self, action: #selector(clean), for: .touchUpInside)
        view.addSubview(btn3)
        btn3.snp.makeConstraints { make in
            make.left.equalTo(btn2.snp.right)
            make.top.equalTo(btn)
            make.width.equalTo(100)
            make.height.equalTo(44)
        }
        
    }

    @objc private func start(){
        ScavengerService.sharedInstance().start()
        
        dispatchTimer(timeInterval: 0.3, repeatCount: 100) { [weak self] timer, count in
            self?.filter()
        }
    }
    
    @objc private func filter(){

        ScavengerService.sharedInstance().filter(withTimestamp: 231, latitude: 22.22, longitude: 23.23) { (result) in

            print("Timestamp>>\(result.timestamp) latitude>>\(result.latitude) longitude>>\(result.longitude) type>>\(result.type) ")
        }
        
    }
    
    @objc private func clean(){
        ScavengerService.sharedInstance().clean()
    }
    
    //MARK:  GCD定时器倒计时
        ///
        /// - Parameters:
        ///   - timeInterval: 间隔时间
        ///   - repeatCount: 重复次数
        ///   - handler: 循环事件,闭包参数: 1.timer 2.剩余执行次数
        func dispatchTimer(timeInterval: Double, repeatCount: Int, handler: @escaping (DispatchSourceTimer?, Int) -> Void) {
            
            if repeatCount <= 0 {
                return
            }
            if timeObserve != nil {
                timeObserve.cancel()//销毁旧的
            }
            // 初始化DispatchSourceTimer前先销毁旧的，否则会存在多个倒计时
            let timer = DispatchSource.makeTimerSource(flags: [], queue: DispatchQueue.main)
            timeObserve = timer
            var count = repeatCount
            timer.schedule(deadline: .now(), repeating: timeInterval)
            timer.setEventHandler {
                count -= 1
                DispatchQueue.main.async {
                    handler(timer, count)
                }
                if count == 0 {
                    timer.cancel()
                }
            }
            timer.resume()
            
        }
    
}

