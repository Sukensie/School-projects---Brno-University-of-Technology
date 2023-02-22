import SwiftUI
import UIKit

struct AllTimersView: View {
    @Binding var timers: [FocusTimer]
    @Environment(\.scenePhase) private var scenePhase
    @State private var showNewTimerView = false
    @State private var newTimerData = FocusTimer.Data()
    @State var activeNavigationLink: String? = nil
    
    let saveAction: ()->Void
    
    //funkce zajišťující správné odkazy u položek listu, isActive na navigationLinku bez této funkce pokazilo odkazování a pokaždé to odkázalo na jinou položku
    func bindingForTimer(timer: String) -> Binding<Bool> {
           .init {
               activeNavigationLink == timer
           } set: { newValue in
               activeNavigationLink = newValue ? timer : nil
           }
       }
    
    
    var body: some View {
        
        ZStack(alignment: .trailing) {
            List {
                ForEach($timers) { $timer in
                    NavigationLink(destination: DetailView(timer: $timer, activeNavigationLink: $activeNavigationLink), isActive: bindingForTimer(timer: timer.title)) {
                        CardView(timer: timer)
                    }
                    .listRowBackground(Color(timer.color))
                    
                }.onDelete { indexSet in
                    timers.remove(atOffsets: indexSet)
                  }
            }

            .navigationTitle("Timers list")
            .sheet(isPresented: $showNewTimerView) {
                NavigationView {
                    DetailEditView(data: $newTimerData)
                        .toolbar {
                            ToolbarItem(placement: .cancellationAction) {
                                Button("Dismiss") {
                                    showNewTimerView = false
                                    newTimerData = FocusTimer.Data()
                                }
                            }
                            ToolbarItem(placement: .confirmationAction) {
                                Button("Add") {
                                    let newTimer = FocusTimer(data: newTimerData)
                                    timers.append(newTimer)
                                    showNewTimerView = false
                                    newTimerData = FocusTimer.Data()
                                }
                            }
                        }
                }
            }
            .onChange(of: scenePhase) { phase in
                if phase == .inactive { saveAction() }
            }
            
            //tlačítko "+" na přidání nového timeru
            VStack {
                Spacer()
                HStack {
                    Spacer()
                    Button(action: {
                        showNewTimerView = true
                    }) {
                    Image(systemName: "plus")
                        .frame(width: 65, height: 65)
                        .foregroundColor(Color.white)
                        .background(Color.green)
                        .clipShape(Circle())
                        .padding()
                        .shadow(color: Color.black.opacity(0.3),
                                radius: 3,
                                x: 3,
                                y: 3)
                    }
                }.padding()
            }
        }
    }
}

