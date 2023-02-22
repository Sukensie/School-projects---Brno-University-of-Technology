import SwiftUI

struct ColorPicker: View {

    @Binding var selection: String

    var body: some View {

        let swatches = [
            "swatch_chestnutrose",
            "swatch_japonica",
            "swatch_rawsienna",
            "swatch_tussock",
            "swatch_asparagus",
            "swatch_patina",
            "swatch_bermudagray",
            "swatch_shipcove",
            "swatch_articblue",
            "swatch_wisteria",
            "swatch_mulberry",
            "swatch_charm",
            "swatch_oslogray",
            "swatch_gunsmoke",
            "swatch_schooner"
        ]

        let columns = [
            GridItem(.adaptive(minimum: 60))
        ]

        LazyVGrid(columns: columns, spacing: 10) {
            ForEach(swatches, id: \.self){ swatch in
                ZStack {
                    Circle()
                        .fill(Color(swatch))
                        .frame(width: 40, height: 40)
                        .onTapGesture(perform: {
                            selection = swatch
                        })
                        .padding(5)

                    if selection == swatch {
                        Circle()
                            .stroke(Color(swatch), lineWidth: 5)
                            .frame(width: 55, height: 55)
                    }
                }
            }
        }
        .padding(10)
    }
}
