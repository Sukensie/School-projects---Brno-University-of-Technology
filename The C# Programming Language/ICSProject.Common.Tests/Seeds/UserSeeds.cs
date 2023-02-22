using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests.Seeds;

public static class UserSeeds
{
    public static readonly UserEntity EmptyUserEntity = new(
        Id: default,
        Name: default!,
        Email: default!,
        TelephoneNumber: default!,
        Birthday: default,
        Hometown: default!,
        ImageUrl: default!);
    
    public static readonly UserEntity User1 = new(
        Id: Guid.Parse(input: "D9E9A8D8-250B-490F-BB80-2FFE8040BCC8"),
        Name: "User1 Name Seeded",
        Email: "User1 Email seeded",
        TelephoneNumber: "User1 telephone seeded",
        Birthday: new DateTime(2000, 1, 1),
        Hometown: "User1 town seeded",
        ImageUrl: null);

    static UserSeeds()
    {
        UserWithCar.Cars.Add(CarSeeds.Car1);
        UserWithCar.Cars.Add(CarSeeds.Car2);
        Driver_NoPassengers.Cars.Add(CarSeeds.Car_RideWithoutPassengers);
        Driver_NoPassengersDelete.Cars.Add(CarSeeds.Car_RideWithout_Delete);
        Driver_WithPassengers.Cars.Add(CarSeeds.Car_RideWithPassengers);
        Driver_WithPassengersDelete.Cars.Add(CarSeeds.Car_RideWith_Delete);
        User1Update.Cars.Add(CarSeeds.Car1Update);
        User1Delete.Cars.Add(CarSeeds.Car1Delete);

        UserWithCar.DriverRides.Add(RideSeeds.RideWithSemistop);
        Driver_NoPassengers.DriverRides.Add(RideSeeds.RideWithPassenger);
        Driver_NoPassengersDelete.DriverRides.Add(RideSeeds.RideWithoutPassengerDelete);
        Driver_WithPassengers.DriverRides.Add(RideSeeds.RideWithPassenger);
        Driver_WithPassengersDelete.DriverRides.Add(RideSeeds.RideWithPassengerDelete);
        UserWithCar.DriverRides.Add(RideSeeds.RideWithSemistopDelete);

        PassengerUser.PassengerRides.Add(PassengerRideSeeds.PassengerRide);
        PassengerUser.PassengerRides.Add(PassengerRideSeeds.PassengerRideUpdate);
        PassengerDelete.PassengerRides.Add(PassengerRideSeeds.PassengerRideDelete);
    }


    public static readonly UserEntity User1Update = new(
        Id: Guid.Parse("143332B9-080E-4953-AEA5-BEF64679B052"),
        Name: "User1 Name Seeded",
        Email: "User1 Email seeded",
        TelephoneNumber: "User1 telephone seeded",
        Birthday: new DateTime(2000, 1, 1),
        Hometown: "User1 town seeded",
        ImageUrl: null
    );

    public static readonly UserEntity User1Delete = new ( 
            Id : Guid.Parse("274D0CC9-A948-4818-AADB-A8B4C0506619"),
            Name: "User1 Name Seeded",
            Email: "User1 Email seeded",
            TelephoneNumber: "User1 telephone seeded",
            Birthday: new DateTime(2000, 1, 1),
            Hometown: "User1 town seeded",
            ImageUrl: null);

    public static readonly UserEntity
        User1DeleteById = User1 with { Id = Guid.Parse("33B3CBC3-AD0B-4F4B-9D12-E87C05F9ED6A") };

    public static readonly UserEntity UserWithCar = new(
        Id: Guid.Parse("A6B261CC-E21B-42D8-8816-C8CC8894457E"),
        Name: "User with car Name Seeded",
        Email: "User with car Email seeded",
        TelephoneNumber: "User with car telephone seeded",
        Birthday: new DateTime(2000, 1, 1),
        Hometown: "User with car town seeded",
        ImageUrl: null);
    

    public static readonly UserEntity PassengerUser = new(
        Id : Guid.Parse("2411230C-F8E4-4712-BA8A-345A1968193A"),
        Name : "Passenger Seeded",
        TelephoneNumber : "911911911",
        Email : "Passenger email",
        Hometown : "Passenger hometown",
        Birthday: new DateTime(2001,1,1),
        ImageUrl:null);

        public static readonly UserEntity PassengerDelete = new(
        Id : Guid.Parse("3C87205C-24F9-4BB9-98C3-820EDA8B522B"),
        Name: "Passenger Seeded",
        TelephoneNumber: "911911911",
        Email: "Passenger email",
        Hometown: "Passenger hometown",
        Birthday: new DateTime(2001, 1, 1),
        ImageUrl: null
        );

    public static readonly UserEntity Driver_NoPassengers = new(
            Id : Guid.Parse("11475D5A-B3C1-45F3-BC44-F84358ACA273"),
            Name : "Driver1 Seeded",
            TelephoneNumber : "911911911",
            Email : "Driver1 email",
            Hometown : "Driver1 hometown",
            Birthday: new DateTime(2001,1,1),
            ImageUrl:null
        );

    public static readonly UserEntity
        Driver_NoPassengersDelete =  new(
            Id : Guid.Parse("8F4B691F-B962-4440-9A78-60D09E7FACBE"),
            Name : "Driver1 Seeded",
            TelephoneNumber : "911911911",
            Email : "Driver1 email",
            Hometown : "Driver1 hometown",
            Birthday: new DateTime(2001,1,1),
            ImageUrl:null
        );

    public static readonly UserEntity
        Driver_WithPassengers = new(
            Id: Guid.Parse("254BE821-3984-4BF4-9043-B13EDB0891EE"),
            Name: "Driver2 Seeded",
            Hometown: "Driver2 Hometown",
            Email: "Driver2 email",
            TelephoneNumber: "911111111",
            Birthday: default,
            ImageUrl: null
        );


    public static readonly UserEntity
        Driver_WithPassengersDelete = new (
            Id : Guid.Parse("EF090B22-4121-470A-8F10-F5775A7E16BF"),
            Name : "Driver2 Seeded",
            Hometown : "Driver2 Hometown",
            Email : "Driver2 email",
            TelephoneNumber: "911111111",
            Birthday:default,
            ImageUrl:null
        );

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<UserEntity>().HasData(
            User1 with {Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>()},
            User1Update with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            User1Delete with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            User1DeleteById with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            UserWithCar with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            PassengerUser with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            PassengerDelete with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            Driver_NoPassengers with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            Driver_WithPassengers with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            Driver_NoPassengersDelete with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() },
            Driver_WithPassengersDelete with { Cars = Array.Empty<CarEntity>(), DriverRides = Array.Empty<RideEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>() }
            );
    }
}