using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.Seeds;

public static class RideSeeds
{
    public static readonly RideEntity RideLatalZlinPraha = new(
        Id: Guid.Parse(input: "73908771-E1FD-4628-A92E-EBDDFA71B3B9"),
        StarTime: new DateTime(2022, 4, 8, 12, 0, 0),
        EndTime: new DateTime(2022, 4, 8, 14, 20, 0),
        Start: "Zlín",
        End: "Praha",
        Price: 120,
        CarId: CarSeeds.Pezot.Id,
        DriverId: UserSeeds.MartinLatal.Id
    )
    {
        Car = CarSeeds.Pezot,
        Driver = UserSeeds.MartinLatal,
    };


    public static readonly RideEntity RideMartinecBrnoBlansko = new(
        Id: Guid.Parse(input:"ED6A9AF0-9387-44FE-BF86-AE7775CC840C"),
        StarTime: new DateTime(2022, 6, 3, 17, 30, 0),
        EndTime: new DateTime(2022, 6, 3, 18, 10, 0),
        Start: "Brno",
        End: "Blansko",
        Price: 25,
        CarId: CarSeeds.FiatDoblo.Id,
        DriverId: UserSeeds.AdamMartinec.Id)
    {
        Car = CarSeeds.FiatDoblo,
        Driver = UserSeeds.AdamMartinec
    };

    public static readonly RideEntity RideMartinecBrnoBlansko2 = new(
        Id: Guid.Parse(input: "8A650DD3-AA8F-4919-8CB1-A03F93468046"),
        StarTime: new DateTime(2022, 6, 1, 17, 30, 0),
        EndTime: new DateTime(2022, 6, 1, 18, 10, 0),
        Start: "Brno",
        End: "Blansko",
        Price: 25,
        CarId: CarSeeds.FiatDoblo.Id,
        DriverId: UserSeeds.AdamMartinec.Id)
    {
        Car = CarSeeds.FiatDoblo,
        Driver = UserSeeds.AdamMartinec
    };

    static RideSeeds()
    {
        RideLatalZlinPraha.PassengerRides.Add(PassengerRideSeeds.LatalMartinecRide);
        RideLatalZlinPraha.PassengerRides.Add(PassengerRideSeeds.LatalOndysRide);
        RideLatalZlinPraha.SemiStops.Add(SemiStopSeeds.LatalSemiStop1);
    }

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<RideEntity>().HasData(
            RideLatalZlinPraha with {
                Car = null, 
                Driver = null, 
                PassengerRides = Array.Empty<PassengerRideEntity>(),
                SemiStops = Array.Empty<SemiStopEntity>()
            },
            
            RideMartinecBrnoBlansko with {
                Car = null, 
                Driver = null, 
                PassengerRides = Array.Empty<PassengerRideEntity>(),
                SemiStops = Array.Empty<SemiStopEntity>()
            },

             RideMartinecBrnoBlansko2 with
             {
                 Car = null,
                 Driver = null,
                 PassengerRides = Array.Empty<PassengerRideEntity>(),
                 SemiStops = Array.Empty<SemiStopEntity>()
             }
        );
    }
}