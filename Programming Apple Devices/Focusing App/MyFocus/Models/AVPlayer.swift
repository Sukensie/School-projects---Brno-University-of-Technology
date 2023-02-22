import Foundation
import AVFoundation

extension AVPlayer {
    static let focusSound: AVPlayer = {
        guard let url = Bundle.main.url(forResource: "start", withExtension: "mp3") else { fatalError("Failed to find sound file.") }
        return AVPlayer(url: url)
    }()
    
    static let pauseSound: AVPlayer = {
        guard let url = Bundle.main.url(forResource: "end", withExtension: "mp3") else { fatalError("Failed to find sound file.") }
        return AVPlayer(url: url)
    }()
}
