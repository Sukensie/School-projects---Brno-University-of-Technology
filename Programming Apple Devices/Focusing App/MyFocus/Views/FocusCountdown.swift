import SwiftUI

struct FocusCountdown: View {
    let secondsElapsed: Int
    let secondsRemaining: Int
    let color: String
    
    private var colorLight: String {
        return color+"_light"
    }
    private var colorDark: String {
        return color+"_dark"
    }
    
    private var totalSeconds: Int {
        secondsElapsed + secondsRemaining
    }
    private var progress: Double {
        guard totalSeconds > 0 else { return 1 }
        return Double(secondsElapsed) / Double(totalSeconds)
    }
    private var hoursRemaining: Int {
        secondsRemaining / 3600
    }
    private var formattedTime: String {
        "\(hoursRemaining):\((secondsRemaining - (hoursRemaining * 3600)) / 60 ):\((secondsRemaining - (hoursRemaining * 3600)) % 60)"
    }
    
    var body: some View {
        VStack() {
            ZStack
            {
                Circle()
                    .stroke(lineWidth: 20)
                    .foregroundColor(Color(colorLight))
                    .opacity(0.1)
                
                Circle()
                    .trim(from: 0, to: min(progress, 1.0))
                    .stroke(Color(colorDark), style: StrokeStyle(lineWidth: 15.0, lineCap: .round, lineJoin: .round))
                    .rotationEffect(Angle(degrees: 270))
                    .animation(.easeInOut(duration: 1), value: progress)
                
                VStack()
                {
                    Text(formattedTime)
                        .fontWeight(.bold)
                        .font(.system(size: 40))
                    
                    if #available(iOS 15.0, *){
                        HStack(alignment: .center)
                        {
                            Image(systemName: "bell.fill")
                                .opacity(0.7)
                                .font(.caption)
                            Text(Date().addingTimeInterval(TimeInterval(totalSeconds)), format: .dateTime.hour().minute())
                                .font(.caption)
                        }
                    }
                }
                
            }
            .frame(width: 250, height: 250)
            .padding()
        }
        .padding([.top, .horizontal])
    }
}

