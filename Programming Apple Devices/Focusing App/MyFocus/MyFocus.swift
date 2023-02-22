import SwiftUI
import UserNotifications

@main
struct MyFocus: App {
    @StateObject private var store = TimerStorage()
    
    var body: some Scene {
        WindowGroup {
            NavigationView {
                AllTimersView(timers: $store.timers) {
                    TimerStorage.save(timers: store.timers) { result in
                        if case .failure(let error) = result {
                            fatalError(error.localizedDescription)
                        }
                    }
                }
            }
            .onAppear {
                //alert na povolení notifikací
                UNUserNotificationCenter.current().requestAuthorization(options: [.alert, .badge, .sound]) {
                    success, error in
                    if success {
                        print("notification good")
                    }
                    else if let error = error {
                        print(error.localizedDescription)
                    }
                }
                //načtení dat
                TimerStorage.load { result in
                    switch result {
                    case .failure(let error):
                        fatalError(error.localizedDescription)
                    case .success(let timers):
                        store.timers = timers
                    }
                }
            }
        }
    }
}
