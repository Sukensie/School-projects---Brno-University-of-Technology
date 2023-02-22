import SwiftUI

struct CardView: View {
    let timer: FocusTimer
    
    var body: some View {
            HStack {
                Image(systemName: timer.icon)
                    .font(.system(size: 20) .bold())
                Text(timer.title)
                    .font(.headline)
                Spacer()
                Text("\(timer.focusLengthInMinutes) min")
                    .font(.system(size: 15))
            }
            .font(.caption)
            .padding(.vertical)
            .foregroundColor(Color.white)
    }
}
