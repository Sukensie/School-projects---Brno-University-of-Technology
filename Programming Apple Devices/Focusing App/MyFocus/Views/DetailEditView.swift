import SwiftUI
    
struct DetailEditView: View {
    @Binding var data: FocusTimer.Data
    
    var body: some View {
        Form {
            Section(header: Text("Timer Info")) {
                VStack (alignment: .leading) {
                    Text("Timer name")
                        .fontWeight(.light)
                        .opacity(0.5)
                    TextField("Title", text: $data.title)
                }.padding(.top, 5.0)
               
                VStack (alignment: .leading) {
                    Text("Focus length")
                        .fontWeight(.light)
                        .opacity(0.5)
                    HStack {
                        Slider(value: $data.focusLengthInMinutes, in: 1...120, step: 1) {
                            Text("Length")
                        }
                        Spacer()
                        Text("\(Int(data.focusLengthInMinutes)) minutes")
                            .accessibilityHidden(true)
                    }
                }.padding(.top, 5.0)
                VStack (alignment: .leading) {
                    Text("Pause length")
                        .fontWeight(.light)
                        .opacity(0.5)
                    HStack {
                        Slider(value: $data.pauseLengthInMinutes, in: 1...60, step: 1) {
                            Text("Length")
                        }
                        Spacer()
                        Text("\(Int(data.pauseLengthInMinutes)) minutes")
                            .accessibilityHidden(true)
                    }
                }.padding(.top, 5.0)
                VStack (alignment: .leading) {
                    Text("Repeat times")
                        .fontWeight(.light)
                        .opacity(0.5)
                    HStack {
                        Slider(value: $data.repeatCount, in: 0...10, step: 1) {
                            Text("Length")
                        }
                        Spacer()
                        Text("\(Int(data.repeatCount))×")
                            .accessibilityHidden(true)
                    }
                }.padding(.top, 5.0)
                
                VStack (alignment: .leading) {
                    Text("Color theme")
                        .fontWeight(.light)
                        .opacity(0.5)
                    ColorPicker(selection: $data.color)
                }.padding(.top, 5.0)
                
                VStack (alignment: .leading) {
                    Text("Icon")
                        .fontWeight(.light)
                        .opacity(0.5)
                    IconPicker(selection: $data.icon)
                }.padding(.top, 5.0)
            }
        }
    }
}
