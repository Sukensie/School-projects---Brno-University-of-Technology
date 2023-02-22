import Foundation

struct FocusTimer: Identifiable, Codable {
    let id: UUID
    var title: String
    var focusLengthInMinutes: Int
    var pauseLengthInMinutes: Int
    var repeatCount: Int
    var icon: String
    var color: String
    var totalFocusLengthInMinutes: Int
    
    init(id: UUID = UUID(), title: String, attendees: [String], focusLengthInMinutes: Int, pauseLengthInMinutes: Int, repeatCount: Int, icon: String, color: String, totalFocusLengthInMinutes: Int) {
        self.id = id
        self.title = title
        self.focusLengthInMinutes = focusLengthInMinutes
        self.pauseLengthInMinutes = pauseLengthInMinutes
        self.repeatCount = repeatCount
        self.icon = icon
        self.color = color
        self.totalFocusLengthInMinutes = totalFocusLengthInMinutes
    }
}

extension FocusTimer {
    
    struct Data {
        var title: String = ""
        var focusLengthInMinutes: Double = 5 //double kvůli pickeru v detailEditView
        var pauseLengthInMinutes: Double = 1
        var repeatCount: Double = 0
        var icon: String = "timer"
        var color: String = "swatch_tussock"
        var totalFocusLengthInMinutes: Int = 0
    }
    
    var data: Data {
        Data(title: title, focusLengthInMinutes: Double(focusLengthInMinutes), pauseLengthInMinutes: Double(pauseLengthInMinutes), repeatCount: Double(repeatCount), icon: icon, color: color, totalFocusLengthInMinutes: totalFocusLengthInMinutes)
    }
    
    mutating func update(from data: Data) {
        title = data.title
        focusLengthInMinutes = Int(data.focusLengthInMinutes)
        pauseLengthInMinutes = Int(data.pauseLengthInMinutes)
        repeatCount = Int(data.repeatCount)
        icon = data.icon
        color = data.color
        totalFocusLengthInMinutes = data.totalFocusLengthInMinutes
    }
    
    init(data: Data) {
        id = UUID()
        title = data.title
        focusLengthInMinutes = Int(data.focusLengthInMinutes)
        pauseLengthInMinutes = Int(data.pauseLengthInMinutes)
        repeatCount = Int(data.repeatCount)
        icon = data.icon
        color = data.color
        totalFocusLengthInMinutes = data.totalFocusLengthInMinutes
    }
}
