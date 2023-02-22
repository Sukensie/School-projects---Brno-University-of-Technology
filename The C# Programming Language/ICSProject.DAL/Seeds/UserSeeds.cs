using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.Seeds;

public static class UserSeeds
{
    public static readonly UserEntity AdamMartinec = new(
        Id: Guid.Parse(input: "B2D0AAF7-95E5-48F0-B367-C1FE8A7E1B13"),
        Name: "Adam Martinec",
        Email: "martinec@latal.com",
        TelephoneNumber: "+420420420420",
        Birthday: new DateTime(2000, 12, 11),
        Hometown: "Lutonina",
        ImageUrl: @"https://is.muni.cz/do/rect/el/estud/pedf/js19/gymnastika_deti/web/pics/akrobacie_kotoul_vzad.jpg");

    public static readonly UserEntity OndysVeca = new(
        Id: Guid.Parse(input: "6EE361E0-6ABD-42A3-9809-FB406E2DE25B"),
        Name: "Ondys Véča",
        Email: "alealeale@zleanelit.team",
        TelephoneNumber: "696969696",
        Birthday: new DateTime(2000, 9, 24), 
        Hometown: "Praha",
        ImageUrl: @"https://scontent-prg1-1.xx.fbcdn.net/v/t1.6435-9/41948585_2644910795734808_3014061169041211392_n.jpg?_nc_cat=111&ccb=1-7&_nc_sid=09cbfe&_nc_ohc=ZIvYAQIaTzwAX8MDlu1&tn=WRl4rSiP6p515IhQ&_nc_ht=scontent-prg1-1.xx&oh=00_AT8UHSnDl5oS4CehL-l5fjt20bHy05-jWuY5h-BaZGwNDQ&oe=62C5F6D8");

    public static readonly UserEntity MartinLatal = new(
        Id: Guid.Parse(input: "6A1CD002-1D5A-4BE3-BB85-9F12A312193D"),
        Name: "Martin Látal",
        Email: "latalm@sohorny.now",
        TelephoneNumber: "100419980",
        Birthday: new DateTime(2001, 10, 4),
        Hometown: "Praha",
        ImageUrl: @"https://i1.sndcdn.com/avatars-TLqzS5E0Fb98ibbi-yyIc2w-t500x500.jpg");

    static UserSeeds()
    {
        //MartinLatal.DriverRides.Add(RideSeeds.RideLatalZlinPraha);
        OndysVeca.PassengerRides.Add(PassengerRideSeeds.LatalOndysRide);
        AdamMartinec.PassengerRides.Add(PassengerRideSeeds.LatalMartinecRide);
        AdamMartinec.DriverRides.Add(RideSeeds.RideMartinecBrnoBlansko);
    }

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<UserEntity>().HasData(
            MartinLatal with
            {
                Cars = Array.Empty<CarEntity>(),
                DriverRides = Array.Empty<RideEntity>(),
                PassengerRides = Array.Empty<PassengerRideEntity>()
            },
            OndysVeca with
            {
                Cars = Array.Empty<CarEntity>(),
                DriverRides = Array.Empty<RideEntity>(),
                PassengerRides = Array.Empty<PassengerRideEntity>()
            },
            AdamMartinec with
            {
                Cars = Array.Empty<CarEntity>(),
                DriverRides = Array.Empty<RideEntity>(),
                PassengerRides = Array.Empty<PassengerRideEntity>()
            }
        );
    }
}