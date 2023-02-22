//
//  Icon.swift
//  MyFocus
//
//  Created by Tom Souček on 24.04.2022.
//

import SwiftUI

enum Icon: String, CaseIterable, Identifiable, Codable {
    case timer
    case trash
    case book
    case desktopcomputer
    case gamecontroller
    case network
    case pencil
    
    var name: String {
        rawValue
    }
    var id: String {
        name
    }
}

