import SwiftUI

struct IconPicker: View {

    @Binding var selection: String

    var body: some View {

        let icons = [
            "timer",
            "book",
            "trash",
            "desktopcomputer",
            "gamecontroller",
            "network",
            "pencil",
            "figure.walk",
            "pawprint.fill",
            "ladybug.fill",
            "cart"
        ]

        let columns = [
            GridItem(.adaptive(minimum: 60))
        ]
        
        LazyVGrid(columns: columns, spacing: 10) {
            ForEach(icons, id: \.self){ icon in
                ZStack {
                    Image(systemName: icon)
                        .onTapGesture(perform: {
                            selection = icon
                        })
                        .padding( 5)

                    if selection == icon {
                        Circle()
                            .stroke(Color.gray, lineWidth: 2)
                            .frame(width: 40, height: 40)
                    }
                }
            }
        }
        .padding(10)
    }
}
