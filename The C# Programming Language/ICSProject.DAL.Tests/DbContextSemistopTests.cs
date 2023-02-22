using System;
using System.Linq;
using System.Threading.Tasks;
using ICSProject.Common.Tests;
using ICSProject.Common.Tests.Seeds;
using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;
using Xunit;
using Xunit.Abstractions;

namespace ICSProject.DAL.Tests
{
    public class DbContextSemiStopTests : DbContextTestsBase
    {
        public DbContextSemiStopTests(ITestOutputHelper output) : base(output)
        {
        }

        [Fact]
        public async Task AddNew_Semistop_Persisted()
        {
            var entity = SemiStopSeeds.EmptySemiStopEntity with
            {
                Id = Guid.Parse("E5235AF4-17BE-4EE3-ABD4-30963934FF99"),
                City = "Hulín",
                Order = 3,
                ArrivalTime = new DateTime(2022, 10, 1, 0, 0, 0),
                RideId = RideSeeds.RideWithSemistop.Id
            };

            //Act
            ICSProjectDbContextSUT.SemiStops.Add(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntities = await dbx.SemiStops.SingleAsync(i => i.Id == entity.Id);

            DeepAssert.Equal(entity, actualEntities);
        }

        [Fact]
        public async Task GetById_Semistop_SemistopRetrieved()
        {
            //Act
            var entity = await ICSProjectDbContextSUT.SemiStops.SingleAsync(i => i.Id == SemiStopSeeds.SemiStop.Id);

            //Assert
            DeepAssert.Equal(SemiStopSeeds.SemiStop with { Ride = null }, entity);
        }

        [Fact]
        public async Task GetAll_SemiStops_ForRide()
        {
            //Act
            var SemiStops = await ICSProjectDbContextSUT.SemiStops
                .Where(i => i.RideId == RideSeeds.RideWithSemistop.Id)
                .ToArrayAsync();

            //Assert
            Assert.Equal(3, SemiStops.Length);
            for (int i = 0; i < SemiStops.Length; i++)
            {
                DeepAssert.Equal(SemiStopSeeds.SemiStop.RideId, SemiStops[i].RideId);
            }
        }

        [Fact]
        public async Task Update_SemiStop_Persisted()
        {
            //Arrange
            var baseEntity = SemiStopSeeds.SemiStopUpdate with {Ride = null};
            var entity =
                baseEntity with
                {
                    City = baseEntity + " UPDATED",
                    Order = 10
                };

            //Act
            ICSProjectDbContextSUT.SemiStops.Update(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntity = await dbx.SemiStops.SingleAsync(i => i.Id == entity.Id);
            DeepAssert.Equal(entity, actualEntity);
        }

        [Fact]
        public async Task Delete_SemiStop_Deleted()
        {
            //Arrange
            var baseEntity = SemiStopSeeds.SemiStopDelete with { Ride = null };

            //Act
            ICSProjectDbContextSUT.SemiStops.Remove(baseEntity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.SemiStops.AnyAsync(i => i.Id == baseEntity.Id));
        }

        [Fact]
        public async Task DeleteById_SemiStop_Deleted()
        {
            //Arrange
            var baseEntity = SemiStopSeeds.SemiStopDelete with { Ride = null };

            //Act
            ICSProjectDbContextSUT.Remove(
                ICSProjectDbContextSUT.SemiStops.Single(i => i.Id == baseEntity.Id));
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == baseEntity.Id));
        }

        [Fact]
        public async Task DeleteRide_SemiStopDeleted()
        {
            await using var dbx = await DbContextFactory.CreateDbContextAsync();

            var observed_entity = SemiStopSeeds.SemiStopDelete with { Ride = null };

            //this Ride has relation with SemiStopDelete
            var entity = RideSeeds.RideWithSemistopDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            var SemiStop_should_exist = await dbx.SemiStops.SingleAsync(i => i.Id == observed_entity.Id);
            DeepAssert.Equal(observed_entity, SemiStop_should_exist);

            Assert.Equal(observed_entity.RideId, entity.Id);

            ICSProjectDbContextSUT.Rides.Remove(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //SemiStop should be also removed after Ride deletion
            Assert.False(await ICSProjectDbContextSUT.SemiStops.AnyAsync(i => i.Id == observed_entity.Id));
        }

    }
}
