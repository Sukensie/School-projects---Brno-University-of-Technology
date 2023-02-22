import SwiftUI
import AVFoundation
import UserNotifications

struct FocusView: View {
    @Binding var timer: FocusTimer
    @StateObject var timerCountdown = TimerCountdown()
    
    //uchovává počet opakování timeru pro násladné porovnání s požadovaným počtem opakování
    @State var currentCount: Int = 0
    
    //přehrávání zvuků
    private var playerFocus: AVPlayer { AVPlayer.focusSound }
    private var playerPause: AVPlayer { AVPlayer.pauseSound }
    
    private var colorDark: String {
        return timer.color+"_dark"
    }
    
    //zapnutí notifikace zobrazující se po konci časovače
    func notification (type: String) {
        let notification = UNMutableNotificationContent()
        notification.title = "MyFocus"
        notification.sound = UNNotificationSound.default
        
        let trigger: UNTimeIntervalNotificationTrigger
        
        if type == "focus" {
            notification.subtitle = "Take a break!"
            trigger = UNTimeIntervalNotificationTrigger(timeInterval: TimeInterval(timer.focusLengthInMinutes * 60), repeats: false)
        }
        else {
            notification.subtitle = "Let's focus!"
            trigger = UNTimeIntervalNotificationTrigger(timeInterval: TimeInterval(timer.pauseLengthInMinutes * 60), repeats: false)
        }
        
        let request = UNNotificationRequest(identifier: UUID().uuidString, content: notification, trigger: trigger)
        
        UNUserNotificationCenter.current().add(request)
    }
    

    var body: some View {
        ZStack {
            Color(timer.color)
                .ignoresSafeArea()
                .overlay(
                    VStack {
                        if timerCountdown.focus == true {
                            if timerCountdown.secondsRemaining <= 0 {
                                Text("Focus number \(currentCount+1) ended")
                                    .fontWeight(.bold)
                                    .font(.title)
                                    .padding(.bottom, 20)
                            }
                            else {
                                Text("Let's focus!")
                                    .fontWeight(.bold)
                                    .font(.title)
                                    .padding(.bottom, 20)
                            }
                        }
                        else {
                            if timerCountdown.secondsRemaining <= 0 && timer.repeatCount <= currentCount {
                                Text("Congrats session finised!")
                                    .fontWeight(.bold)
                                    .font(.title)
                                    .padding(.bottom, 20)
                            }
                                
                            else {
                                if timerCountdown.secondsRemaining <= 0 {
                                    Text("Break number \(currentCount+1) ended")
                                        .fontWeight(.bold)
                                        .font(.title)
                                        .padding(.bottom, 20)
                                }
                                else {
                                Text("Take a break!")
                                    .fontWeight(.bold)
                                    .font(.title)
                                    .padding(.bottom, 20)
                                }
                            }
                        }
                        
                        //countdown progress circle
                        FocusCountdown(secondsElapsed: timerCountdown.secondsElapsed, secondsRemaining: timerCountdown.secondsRemaining, color: timer.color).padding(.bottom, 30.0)
                        
                        //tlačítko na potrvzení konce soustředění/pauzy
                        Button("Confirm")
                        {
                            timerCountdown.focus.toggle()
                            
                            if(timerCountdown.focus) {
                                playerFocus.seek(to: CMTime.zero)
                                playerFocus.play()
                                timerCountdown.reset(lengthInMinutes: timer.focusLengthInMinutes)
                                notification(type: "focus")
                            }
                            else {
                                currentCount += 1
                                playerPause.seek(to: CMTime.zero)
                                playerPause.play()
                                timerCountdown.reset(lengthInMinutes: timer.pauseLengthInMinutes)
                                notification(type: "pause")
                                timer.totalFocusLengthInMinutes += timer.focusLengthInMinutes
                            }
                        }
                        .disabled(timerCountdown.secondsRemaining <= 0 && timer.repeatCount >= currentCount ? false : true)
                        .foregroundColor(.white)
                        .padding()
                        .background(Color(colorDark))
                        .opacity(timerCountdown.secondsRemaining <= 0 && timer.repeatCount >= currentCount ? 1 : 0)
                        .cornerRadius(10)
                    }
                )
        }
        .foregroundColor(Color.white)
        .onAppear {
            playerFocus.seek(to: CMTime.zero)
            playerFocus.play()
            timerCountdown.reset(lengthInMinutes: timer.focusLengthInMinutes)
            notification(type: "focus")
        }
        .onDisappear {
            timerCountdown.stopTimer()
            UNUserNotificationCenter.current().removeAllPendingNotificationRequests()
        }
        .navigationBarTitleDisplayMode(.inline)
    }
}

