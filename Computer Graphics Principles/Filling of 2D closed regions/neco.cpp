void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow)
{
    // Nalezeni obalky (minX, maxX), (minY, maxY) trojuhleniku.

    //////// DOPLNTE KOD /////////
    auto minx = MIN(MIN(v1.x, v2.x), v3.x);
    auto miny = MIN(MIN(v1.y, v2.y), v3.y);
    
    auto maxx = MAX(MAX(v1.x, v2.x), v3.x);
    auto maxy = MAX(MAX(v1.y, v2.y), v3.y);


    // Oriznuti obalky (minX, maxX, minY, maxY) trojuhleniku podle rozmeru okna.

    //////// DOPLNTE KOD /////////
    minx = MAX(minx, 0);
	maxx = MIN(maxx, width -1);

	miny = MAX(miny, 0);
	maxy = MIN(maxy, height -1);


    // Spocitani parametru hranove funkce (deltaX, deltaY) pro kazdou hranu.
	// Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
	// v prvnim vrcholu hrany, konec v druhem vrcholu.
	// Vypocet prvnotni hodnoty hranove funkce.

    //////// DOPLNTE KOD /////////
    const auto dx1 = v2.x - v1.x;
    const auto dx2 = v3.x - v2.x;
    const auto dx3 = v1.x - v3.x;
    
    const auto dy1 = v2.y - v1.y;
    const auto dy2 = v3.y - v2.y;
    const auto dy3 = v1.y - v3.y;


    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y).

    //////// DOPLNTE KOD /////////
    auto x = minx;
    auto y = miny;
    // E(x,y) = (y - y0)dx - (x - x0)dy
    auto E1 = (y - v1.y)*dx1 - (x - v1.x)*dy1;
    auto E2 = (y - v2.y)*dx2 - (x - v2.x)*dy2;
    auto E3 = (y - v3.y)*dx3 - (x - v3.x)*dy3;

    for (; y < maxy; ++y, E1 += dx1, E2 += dx2, E3 += dx3)
    {
        bool even = (y - miny) % 2 == 0;

        int startX = even ? minx : maxx;
        int endX   = even ? maxx + 1 : minx - 1;
        int stepX  = even ? 1 : -1; 

        for (x = startX; x != endX; x += stepX) {
            if (E1 >= 0 && E2 >= 0 && E3 >= 0) 
                putPixel(x,y,color1);
            if (x != endX - stepX) {
                E1 += even ? -dy1 : dy1;
                E2 += even ? -dy2 : dy2;
                E3 += even ? -dy3 : dy3;
            }
        }
    }
    
    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2, arrow);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2, arrow);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2, arrow);
}