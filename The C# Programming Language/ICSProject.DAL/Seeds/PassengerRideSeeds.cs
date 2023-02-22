using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.Seeds;

public static class PassengerRideSeeds
{
    public static readonly PassengerRideEntity LatalMartinecRide = new(
        Id: Guid.Parse("A15E6A9E-4940-4E7B-8FA9-64AB00B7C5ED"),
        PassengerId: UserSeeds.AdamMartinec.Id,
        RideId: RideSeeds.RideLatalZlinPraha.Id)
    {
        Passenger = UserSeeds.AdamMartinec,
        Ride = RideSeeds.RideLatalZlinPraha
    };

    public static readonly PassengerRideEntity MartinecOndysRide = new(
        Id: Guid.Parse("621D04E7-C415-4D53-A14A-FDDA5E9927F3"),
        PassengerId: UserSeeds.OndysVeca.Id,
        RideId: RideSeeds.RideMartinecBrnoBlansko.Id)
    {
        Passenger = UserSeeds.OndysVeca,
        Ride = RideSeeds.RideMartinecBrnoBlansko
    };

    public static readonly PassengerRideEntity LatalOndysRide = new(
        Id: Guid.Parse("F9B71B96-B008-47FB-860E-41711386D0D6"),
        PassengerId: UserSeeds.OndysVeca.Id,
        RideId: RideSeeds.RideLatalZlinPraha.Id)
    {
        Passenger = UserSeeds.OndysVeca,
        Ride = RideSeeds.RideLatalZlinPraha
    };

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<PassengerRideEntity>().HasData(
            LatalMartinecRide with{Passenger = null, Ride = null},
            LatalOndysRide with{Passenger = null, Ride = null}
        );
    }
}