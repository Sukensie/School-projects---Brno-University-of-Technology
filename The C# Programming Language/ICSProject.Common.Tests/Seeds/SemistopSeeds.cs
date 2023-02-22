using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests.Seeds;

public static class SemiStopSeeds
{
    public static readonly SemiStopEntity EmptySemiStopEntity = new(
        Id: default,
        City: default!,
        Order: default,
        ArrivalTime: default,
        RideId: default);

    public static readonly SemiStopEntity SemiStop = new (
        Id : Guid.Parse("6121E804-518E-47C0-B95C-FF56154C4B46"),
        Order : 1,
        RideId : RideSeeds.RideWithSemistop.Id,
        City : "City seeded",
        ArrivalTime : new DateTime(2020, 1, 1, 0, 0, 0)
        )
    {
        Ride = RideSeeds.RideWithSemistop
    };

    public static readonly SemiStopEntity SemiStop2 =
        EmptySemiStopEntity with
        {
            Id = Guid.Parse("7920A1A7-6108-4714-A44C-E06192E3CD6E"),
            Order = 2,
            RideId = RideSeeds.RideWithSemistop.Id,
            City = "City seeded 2",
            ArrivalTime = new DateTime(2021, 1, 1, 0, 0, 0)
        };

    public static readonly SemiStopEntity SemiStopUpdate =
        SemiStop with
        {
            Id = Guid.Parse("972F7630-D54F-4617-B01A-02DE8BB8661A")
        };

    public static readonly SemiStopEntity SemiStopDelete =
        SemiStop with
        {
            Id = Guid.Parse("23BAFFB2-975A-4747-9901-9369D56A7C0A"),
            RideId = RideSeeds.RideWithSemistopDelete.Id,
            Ride = RideSeeds.RideWithSemistopDelete
        };

    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<SemiStopEntity>().HasData(
            SemiStop with{ Ride = null},
            SemiStop2 with { Ride = null },
            SemiStopDelete with { Ride = null },
            SemiStopUpdate with { Ride = null }
        );
    }
    
}