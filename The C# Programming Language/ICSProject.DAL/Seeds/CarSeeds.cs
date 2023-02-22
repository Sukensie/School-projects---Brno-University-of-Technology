using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.Seeds
{
    public static class CarSeeds
    {
        public static readonly CarEntity FiatDoblo = new(
            Id: Guid.Parse(input: "0d4fa150-ad80-4d46-a511-4c666166ec5e"),
            Manufacturer: "Fiat",
            Type: "Doblo",
            RegistrationTime: new DateTime(2020, 5, 1, 8, 30, 52),
            NumberOfSeats: 2,
            ImageUrl: @"https://upload.wikimedia.org/wikipedia/commons/3/35/Overview_of_DoDo_car_at_%C3%9Adoln%C3%AD_street_in_Prague-Bran%C3%ADk%2C_Prague.jpg",
            OwnerId: UserSeeds.AdamMartinec.Id)
        {
            Owner = UserSeeds.AdamMartinec
        };

        public static readonly CarEntity CernySUV = new(
            Id: Guid.Parse("74D853E7-06E2-43B2-9B6F-E022B11F2E1C"),
            Manufacturer: "Audi",
            Type: "Q7 50 TDI Quattro S-LINE",
            RegistrationTime: new DateTime(2021, 5, 4, 3, 2, 1),
            NumberOfSeats: 5,
            ImageUrl: @"https://cdn.myshoptet.com/usr/www.autoibuy.com/user/shop/big/2255-10_q76.png?5fce5b39",
            OwnerId: UserSeeds.OndysVeca.Id)
        {
            Owner = UserSeeds.OndysVeca
        };

        public static readonly CarEntity FordFocus = new(
            Id: Guid.Parse("9C7C3C9F-32AD-44C3-A27C-35C79B1EF0B1"),
            Manufacturer: "Ford",
            Type: "Focus ST",
            RegistrationTime: new DateTime(2019, 6, 12, 13, 2, 35),
            NumberOfSeats: 5,
            ImageUrl: @"https://www.autoweb.cz/wp-content/uploads/2021/04/img_8402-1100x618.jpg",
            OwnerId: UserSeeds.AdamMartinec.Id)
        {
            Owner = UserSeeds.AdamMartinec
        };

        public static readonly CarEntity Pezot = new(
            Id: Guid.Parse("DF985ED3-DBC5-48A8-926E-CA15FA86A738"),
            Manufacturer: "Peugeot",
            Type: "206",
            RegistrationTime: new DateTime(2019, 11, 5, 7, 8, 35),
            NumberOfSeats: 5,
            ImageUrl: @"https://cdn.xsd.cz/resize/a35cccc886033bb5897c7d4bcf06d3c1_resize=1001,722_.jpg?hash=c7d01b1b2949a8bdfedf032a40dab41e",
            OwnerId: UserSeeds.MartinLatal.Id)
        {
            Owner = UserSeeds.MartinLatal
        };

        static CarSeeds()
        {
            Pezot.Rides.Add(RideSeeds.RideLatalZlinPraha);
            FiatDoblo.Rides.Add(RideSeeds.RideMartinecBrnoBlansko);
        }

        public static void Seed(this ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<CarEntity>().HasData(
                FiatDoblo with {Rides = Array.Empty<RideEntity>(), Owner = null},
                Pezot with { Rides = Array.Empty<RideEntity>(), Owner = null},
                CernySUV with { Rides = Array.Empty<RideEntity>(), Owner = null},
                FordFocus with { Rides = Array.Empty<RideEntity>(), Owner = null}
            );
        }
    }
}
