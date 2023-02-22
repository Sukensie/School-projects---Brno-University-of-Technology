//
//  TimersView.swift
//  MyFocus
//
//  Created by Tomáš Souček on 24.04.2022.
//

import SwiftUI
import UIKit

struct TimersView: View {
    @Binding var timers: [FocusTimer]
    @Environment(\.scenePhase) private var scenePhase
    @State private var isPresentingNewTimerView = false
    @State private var newTimerData = FocusTimer.Data()
    @State var showDetail = false
    
    let saveAction: ()->Void
    
    
    var body: some View {
        
        ZStack(alignment: .trailing) {
            List {
                ForEach($timers) { $timer in
                    NavigationLink(destination: DetailView(timer: $timer, showDetail: $showDetail), isActive: $showDetail) {
                        CardView(timer: timer)
                    }
                    .listRowBackground(Color(timer.color))
                    
                }.onDelete { indexSet in
                    timers.remove(atOffsets: indexSet)
                  }
            }
            .navigationTitle("Timers list")
            .sheet(isPresented: $isPresentingNewTimerView) {
                NavigationView {
                    DetailEditView(data: $newTimerData)
                        .toolbar {
                            ToolbarItem(placement: .cancellationAction) {
                                Button("Dismiss") {
                                    isPresentingNewTimerView = false
                                    newTimerData = FocusTimer.Data()
                                }
                            }
                            ToolbarItem(placement: .confirmationAction) {
                                Button("Add") {
                                    let newTimer = FocusTimer(data: newTimerData)
                                    timers.append(newTimer)
                                    isPresentingNewTimerView = false
                                    newTimerData = FocusTimer.Data()
                                }
                            }
                        }
                }
            }
            .onChange(of: scenePhase) { phase in
                if phase == .inactive { saveAction() }
            }
            
            VStack {
                Spacer()
                HStack {
                    Spacer()
                    Button(action: {
                        isPresentingNewTimerView = true
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

