using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests.Seeds;

public static class PassengerRideSeeds
{
    public static readonly PassengerRideEntity EmptyPassengerRideEntity = new(
        Id: default,
        PassengerId: default,
        RideId: default);

    public static readonly PassengerRideEntity PassengerRide = new(
        Id: Guid.Parse("6CB56383-E691-496C-8D94-B72874672748"),
        PassengerId: UserSeeds.PassengerUser.Id,
        RideId: RideSeeds.RideWithPassenger.Id
        )
    {
        Passenger = UserSeeds.PassengerUser,
        Ride = RideSeeds.RideWithPassenger
    };

    public static readonly PassengerRideEntity PassengerRideUpdate = new(
        Id: Guid.Parse("94D506C1-26EE-422B-9504-682679CE91FB"),
        PassengerId: UserSeeds.PassengerUser.Id,
        RideId: RideSeeds.RideWithPassenger.Id)
    {
        Passenger = UserSeeds.PassengerUser,
        Ride = RideSeeds.RideWithPassenger
    };

    public static readonly PassengerRideEntity PassengerRideDelete = new (
        Id : Guid.Parse("8B54B9F6-B334-4A61-9411-EF78467BDF6A"),
        PassengerId: UserSeeds.PassengerDelete.Id,
        RideId: RideSeeds.RideWithPassengerDelete.Id)
    {
        Passenger = UserSeeds.PassengerDelete,
        Ride = RideSeeds.RideWithPassengerDelete
    };

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<PassengerRideEntity>().HasData(
            PassengerRide with{Passenger = null, Ride = null},
            PassengerRideUpdate with { Passenger = null, Ride = null },
            PassengerRideDelete with { Passenger = null, Ride = null }
            );
    }
}