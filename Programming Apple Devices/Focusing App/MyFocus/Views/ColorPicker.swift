//
//  ColorPicker.swift
//  MyFocus
//
//  Created by Tom Souček on 25.04.2022.
//

import SwiftUI

struct ContentView: View {

    @State var selection: String = "swatch_shipcove"

    var body: some View {
        VStack {
            ColorSwatchView(selection: $selection)

            RoundedRectangle(cornerRadius: 25, style: .continuous)
                .fill(Color(selection))
                .padding()
        }
    }
}
