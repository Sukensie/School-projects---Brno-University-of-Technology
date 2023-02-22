using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.Seeds;

public static class SemiStopSeeds
{
    public static readonly SemiStopEntity LatalSemiStop1 = new(
        Id: Guid.Parse("0AF4CD3A-914E-4E95-A0F4-FCA522FA361B"),
        City: "Brno",
        Order: 1,
        ArrivalTime: new DateTime(2022, 4, 8, 13, 20, 0),
        RideId: RideSeeds.RideLatalZlinPraha.Id)
    {
        Ride = RideSeeds.RideLatalZlinPraha
    };
    public static void Seed(this ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<SemiStopEntity>().HasData(
            LatalSemiStop1 with {Ride = null}
        );
    }
}