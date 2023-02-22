//
//  ThemeView.swift
//  MyFocus
//
//  Created by Tom Souček on 24.04.2022.
//

import SwiftUI

struct ThemeView: View {
    let theme: Theme
    
    var body: some View {
        ZStack {
            theme.mainColor
                .ignoresSafeArea()
                .overlay(
                    Text(theme.name)
                        .padding()
                    )
        }
        .foregroundColor(theme.accentColor)
    }
}
