import Foundation


class TimerCountdown: ObservableObject {

    @Published var secondsElapsed = 0
    @Published var secondsRemaining = 0
    @Published var focus = true

    
    private(set) var lengthInMinutes: Int


    private var timer: Timer?
    private var timerStopped = false
    private var frequency: TimeInterval { 1.0 / 60.0 }
    private var lengthInSeconds: Int { lengthInMinutes * 60 }


    private var startDate: Date?
    

    init(lengthInMinutes: Int = 0) {
        self.lengthInMinutes = lengthInMinutes
        secondsRemaining = lengthInSeconds
    }
    
    func startTimer() {
        timerStatus()
    }
    
    func stopTimer() {
        timer?.invalidate()
        timer = nil
        timerStopped = true
    }
    

    private func timerStatus() {

        secondsRemaining = lengthInSeconds - secondsElapsed
        startDate = Date()
        timer = Timer.scheduledTimer(withTimeInterval: frequency, repeats: true) { [weak self] timer in
            if let self = self, let startDate = self.startDate {
                let secondsElapsed = Date().timeIntervalSince1970 - startDate.timeIntervalSince1970
                self.update(secondsElapsed: Int(secondsElapsed))
            }
        }
    }

    private func update(secondsElapsed: Int) {
        self.secondsElapsed = secondsElapsed
        secondsRemaining = max(lengthInSeconds - self.secondsElapsed, 0)
        guard secondsElapsed <= lengthInSeconds else {
            return
        }
        guard !timerStopped else { return }
    }

    func reset(lengthInMinutes: Int) {
        self.lengthInMinutes = lengthInMinutes
        secondsRemaining = lengthInSeconds
        stopTimer()
        startTimer()
    }
}

extension FocusTimer {
    var timer: TimerCountdown {
        return TimerCountdown(lengthInMinutes: focusLengthInMinutes)
    }
}
