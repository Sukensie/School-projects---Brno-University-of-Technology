using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests.Seeds;

public static class RideSeeds
{

    public static readonly RideEntity EmptyRideEntity = new(
        Id: default,
        StarTime: default,
        EndTime: default,
        Start: default!,
        End: default!,
        Price: default,
        CarId: default,
        DriverId: default
    );

    static RideSeeds()
    {
        RideWithSemistop.SemiStops.Add(SemiStopSeeds.SemiStop);
        RideWithSemistopDelete.SemiStops.Add(SemiStopSeeds.SemiStopDelete);
        RideWithSemistop.SemiStops.Add(SemiStopSeeds.SemiStopUpdate);
        RideWithSemistop.SemiStops.Add(SemiStopSeeds.SemiStop2);

        RideWithPassenger.PassengerRides.Add(PassengerRideSeeds.PassengerRide);
        RideWithPassenger.PassengerRides.Add(PassengerRideSeeds.PassengerRideUpdate);
        RideWithPassengerDelete.PassengerRides.Add(PassengerRideSeeds.PassengerRideDelete);

    }

    public static readonly RideEntity RideWithoutPassenger = new(
        Id: Guid.Parse("C685B4AD-69E0-4968-9846-626FC8E8AB14"),
        StarTime: new DateTime(2022, 4, 8, 12, 0, 0),
        EndTime: new DateTime(2022, 4, 8, 14, 20, 0),
        Start: "Ride without Passenger Start seeded",
        End: "Ride1 without Passenger seeded",
        Price: 120,
        CarId: CarSeeds.Car_RideWithoutPassengers.Id,
        DriverId: UserSeeds.Driver_NoPassengers.Id
    )
    {
        Car = CarSeeds.Car_RideWithoutPassengers,
        Driver = UserSeeds.Driver_NoPassengers
    };

    public static readonly RideEntity RideWithoutPassengerDelete = new(
        Id : Guid.Parse("BFC89FCD-B751-4D70-A4C3-BD209D087A3D"),
        DriverId : UserSeeds.Driver_NoPassengersDelete.Id,
        CarId : CarSeeds.Car_RideWithout_Delete.Id,
        StarTime: new DateTime(2022, 4, 8, 12, 0, 0),
        EndTime: new DateTime(2022, 4, 8, 14, 20, 0),
        Start: "Ride without Passenger Start seeded",
        End: "Ride1 without Passenger seeded",
        Price: 120
    )
    {
        Driver = UserSeeds.Driver_NoPassengersDelete,
        Car = CarSeeds.Car_RideWithout_Delete
    };


    public static readonly RideEntity RideWithPassenger = new(
        Id: Guid.Parse("69E82B32-B260-4786-A95D-94C6B41A1572"),
        StarTime: new DateTime(2022, 4, 8, 12, 0, 0),
        EndTime: new DateTime(2022, 4, 8, 14, 20, 0),
        Start: "Ride With Passenger Start seeded",
        End: "Ride1 With Passenger seeded",
        Price: 120,
        CarId: CarSeeds.Car_RideWithPassengers.Id,
        DriverId: UserSeeds.Driver_WithPassengers.Id
    )
    {
        Car = CarSeeds.Car_RideWithPassengers,
        Driver = UserSeeds.Driver_WithPassengers
    };

    public static readonly RideEntity RideWithPassengerDelete = new(
            Id : Guid.Parse("67B89DC5-0134-4FAA-8EBB-691E93D484D0"),
            DriverId : UserSeeds.Driver_WithPassengersDelete.Id,
            CarId : CarSeeds.Car_RideWith_Delete.Id,
            StarTime: new DateTime(2022, 4, 8, 12, 0, 0),
            EndTime: new DateTime(2022, 4, 8, 14, 20, 0),
            Start: "Ride With Passenger Start seeded",
            End: "Ride1 With Passenger seeded",
            Price: 120
    )
    {
        Driver = UserSeeds.Driver_WithPassengersDelete,
        Car = CarSeeds.Car_RideWith_Delete
    };

    public static readonly RideEntity RideWithSemistop = new (
        Id : Guid.Parse("6233BD76-1B18-492E-BFE0-FF8EA4B470A4"),
            Start : "Ride with semistop start",
            End : "Ride with semistop end",
            DriverId : UserSeeds.UserWithCar.Id,
            CarId : CarSeeds.Car1.Id,
            StarTime: default,
            EndTime: default,
            Price: 0
            )
    {
        Driver = UserSeeds.UserWithCar,
        Car = CarSeeds.Car1
    };

    public static readonly RideEntity RideWithSemistopDelete =
        new(
            Id : Guid.Parse("2DD2F3FF-67F6-4ADC-8D55-62A8B99E69E6"),
            Start: "Ride with semistop start",
            End: "Ride with semistop end",
            DriverId: UserSeeds.UserWithCar.Id,
            CarId: CarSeeds.Car1.Id,
            StarTime: default,
            EndTime: default,
            Price: 0)
        {
            Driver = UserSeeds.UserWithCar,
            Car = CarSeeds.Car1
        };

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<RideEntity>().HasData(
            RideWithPassenger with { Driver = null, Car = null, PassengerRides = Array.Empty<PassengerRideEntity>(), SemiStops = Array.Empty<SemiStopEntity>() },
            RideWithPassengerDelete with { Driver = null, Car = null, PassengerRides = Array.Empty<PassengerRideEntity>(), SemiStops = Array.Empty<SemiStopEntity>() },
            RideWithoutPassenger with { Driver = null, Car = null, PassengerRides = Array.Empty<PassengerRideEntity>(), SemiStops = Array.Empty<SemiStopEntity>() },
            RideWithoutPassengerDelete with { Driver = null, Car = null, PassengerRides = Array.Empty<PassengerRideEntity>(), SemiStops = Array.Empty<SemiStopEntity>() },
            RideWithSemistop with {Driver = null, Car = null, PassengerRides = Array.Empty<PassengerRideEntity>(), SemiStops = Array.Empty<SemiStopEntity>()},
            RideWithSemistopDelete with { Driver = null, Car = null, PassengerRides = Array.Empty<PassengerRideEntity>(), SemiStops = Array.Empty<SemiStopEntity>() }
        );
    }
}