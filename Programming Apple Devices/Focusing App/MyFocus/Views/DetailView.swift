import SwiftUI
import UserNotifications

struct DetailView: View {
    @Binding var timer: FocusTimer
    @State private var data = FocusTimer.Data()
    @State private var showEditView = false
    
    @Binding var activeNavigationLink: String? //tím, že níže používám v detailu nové navigation view, vykreslovalo to 2 zpětné odkazy -> zobrazování detail view funguje nyní
    
    var body: some View {
        //navigation view je nutný, aby timer běžel i v pozadí, ale zároveň nechci aby byl vidět -> úpravy navigationBar
        NavigationView {
            List {
                Section(header: Text("Timer Info")) {
                    NavigationLink(destination: FocusView(timer: $timer)) {
                        Label("Start timer", systemImage: "timer")
                            .font(.headline)
                            .foregroundColor(.accentColor)
                    }
                
                    HStack {
                        Label("Focus length", systemImage: "clock")
                        Spacer()
                        Text("\(timer.focusLengthInMinutes) minutes")
                    }
                    HStack {
                        Label("Pause length", systemImage: "clock")
                        Spacer()
                        Text("\(timer.pauseLengthInMinutes) minutes")
                    }
                    HStack {
                        Label("Repeat count", systemImage: "arrow.counterclockwise")
                        Spacer()
                        Text("\(timer.repeatCount)x")
                    }
                    HStack {
                        Label("Icon", systemImage: "eye")
                        Spacer()
                        Image(systemName: timer.icon)
                            .padding(4)
                    }
                    HStack {
                        Label("Color scheme", systemImage: "paintpalette")
                        Spacer()
                        Circle()
                            .fill(Color(timer.color))
                            .frame(width: 20, height: 20)
                    }
                }
                
                Section(header: Text("Stats")) {
                    HStack {
                        Label("Already focused", systemImage: "brain")
                        Spacer()
                        Text("\(timer.totalFocusLengthInMinutes) minutes")
                    }
                    
                }
            }
            .navigationTitle(timer.title)
            .toolbar {
                ToolbarItem (placement: .navigationBarLeading){
                    Button {
                        activeNavigationLink = nil
                    } label: {
                        HStack (spacing: 3.5) {
                            Image(systemName: "chevron.left").font(Font.system(size: 18, weight: .semibold))
                            Text("Timers list")
                        }
                    }
                }
                ToolbarItem (placement: .navigationBarTrailing){
                    Button("Edit") {
                        showEditView = true
                        data = timer.data
                    }
                }
            }
                
        }.navigationBarTitle("")
         .navigationBarHidden(true)
        .sheet(isPresented: $showEditView) {
            NavigationView {
                DetailEditView(data: $data)
                    .navigationTitle(timer.title)
                    .toolbar {
                        ToolbarItem(placement: .cancellationAction) {
                            Button("Cancel") {
                                showEditView = false
                            }
                        }
                        ToolbarItem(placement: .confirmationAction) {
                            Button("Done") {
                                showEditView = false
                                timer.update(from: data)
                            }
                        }
                    }
            }
        }.gesture(DragGesture(minimumDistance: 3.0, coordinateSpace: .local)
            .onEnded { value in
                print(value.translation)
                switch(value.translation.width, value.translation.height) {
                    case (0..., -30...30):  activeNavigationLink = nil //right swipe

                    default: print("nothing")
                }
            }
        )
    }
}
