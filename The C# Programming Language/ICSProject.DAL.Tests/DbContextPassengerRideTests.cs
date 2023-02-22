using System;
using System.Linq;
using System.Threading.Tasks;
using ICSProject.Common.Tests;
using ICSProject.Common.Tests.Seeds;
using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;
using Xunit;
using Xunit.Abstractions;
using Newtonsoft.Json; //pro JSON převody

namespace ICSProject.DAL.Tests
{
    /// <summary>
    /// Tests shows an example of DbContext usage when querying strong entity with no navigation properties.
    /// Entity has no relations, holds no foreign keys.
    /// </summary>
    public class DbContextPassengerRideTests : DbContextTestsBase
    {
        public DbContextPassengerRideTests(ITestOutputHelper output) : base(output)
        {
        }       
        [Fact]
        public async Task AddNew_PassengerRide_Persisted()
        {

            var entity = PassengerRideSeeds.EmptyPassengerRideEntity with
            {
                Id = Guid.Parse("40A94406-211F-44E5-BE9D-9DE7B760B283"),
                PassengerId = UserSeeds.User1.Id,
                RideId = RideSeeds.RideWithPassenger.Id,
                Passenger = null,
                Ride = null
            };

            //Act
            ICSProjectDbContextSUT.PassengerRides.Add(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntities = await dbx.PassengerRides.SingleAsync(i => i.Id == entity.Id);

            DeepAssert.Equal(entity, actualEntities);
        }

        [Fact]
        public async Task GetById_PassengerRide_PassengerRideRetrieved()
        {
            //Act
            var entity = await ICSProjectDbContextSUT.PassengerRides.SingleAsync(i => i.Id == PassengerRideSeeds.PassengerRide.Id);

            //Assert
            Assert.Equal(PassengerRideSeeds.PassengerRide with {Passenger = null, Ride = null}, entity);
        }

        [Fact]
        public async Task GetAll_PassengerRides_ForPassenger()
        {
            //Act
            var PassengerRides = await ICSProjectDbContextSUT.PassengerRides
               .Where(i => i.PassengerId == PassengerRideSeeds.PassengerRide.PassengerId)
               .ToArrayAsync();

            //Assert
            //porovnávám jednolivé složky zvlášť, protože id každé jízdy by se mělo vždy lišit
            for (int i = 0; i < PassengerRides.Length; i++)
            {
                Assert.Equal(PassengerRideSeeds.PassengerRide.PassengerId, PassengerRides[i].PassengerId);
                Assert.Equal(PassengerRideSeeds.PassengerRide.RideId, PassengerRides[i].RideId);
            }
        }

        [Fact]
        public async Task Update_PassengerRide_Persisted()
        {
            //Arrange
            var baseEntity = PassengerRideSeeds.PassengerRideUpdate with {Ride = null, Passenger = null};
            var entity =
                baseEntity with
                {
                    PassengerId = UserSeeds.User1.Id
                };

            //Act
            ICSProjectDbContextSUT.PassengerRides.Update(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntity = await dbx.PassengerRides.SingleAsync(i => i.Id == entity.Id);
            Assert.Equal(entity, actualEntity);
        }


        [Fact]
        public async Task Delete_PassengerRide_PassengerRideDeleted()
        {
            //Arrange
            var entityBase = PassengerRideSeeds.PassengerRideDelete with {Ride = null, Passenger = null};

            //Act
            ICSProjectDbContextSUT.PassengerRides.Remove(entityBase);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.PassengerRides.AnyAsync(i => i.Id == entityBase.Id));
        }

        [Fact]
        public async Task DeleteById_PassengerRide_PassengerRideDeleted()
        {
            //Arrange
            var entityBase = PassengerRideSeeds.PassengerRideDelete with { Ride = null, Passenger = null };

            //Act
            ICSProjectDbContextSUT.Remove(
                ICSProjectDbContextSUT.PassengerRides.Single(i => i.Id == entityBase.Id));
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.PassengerRides.AnyAsync(i => i.Id == entityBase.Id));
        }

        [Fact]
        public async Task DeletePassenger_PassengerRideDeleted()
        {
            await using var dbx = await DbContextFactory.CreateDbContextAsync();

            var observed_entity = PassengerRideSeeds.PassengerRideDelete with { Ride = null, Passenger = null};

            //this passenger has relation with PassengerRideDelete
            var entity = UserSeeds.PassengerDelete with { Cars = Array.Empty<CarEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), DriverRides = Array.Empty<RideEntity>() }; ;

            var PassengerRide_should_exist = await dbx.PassengerRides.SingleAsync(i => i.Id == observed_entity.Id);
            DeepAssert.Equal(observed_entity, PassengerRide_should_exist);

            Assert.Equal(observed_entity.PassengerId, entity.Id);

            ICSProjectDbContextSUT.Users.Remove(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //PassengerRide should be also removed after Passenger deletion
            Assert.False(await ICSProjectDbContextSUT.PassengerRides.AnyAsync(i => i.Id == observed_entity.Id));
        }

        [Fact]
        public async Task DeleteRide_PassengerRideDeleted()
        {
            await using var dbx = await DbContextFactory.CreateDbContextAsync();

            var observed_entity = PassengerRideSeeds.PassengerRideDelete with { Ride = null, Passenger = null };

            //this ride has relation with PassengerRideDelete
            var entity = RideSeeds.RideWithPassengerDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            var PassengerRide_should_exist = await dbx.PassengerRides.SingleAsync(i => i.Id == observed_entity.Id);
            DeepAssert.Equal(observed_entity, PassengerRide_should_exist);

            Assert.Equal(observed_entity.RideId, entity.Id);

            ICSProjectDbContextSUT.Rides.Remove(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //PassengerRide should be also removed after Ride deletion
            Assert.False(await ICSProjectDbContextSUT.PassengerRides.AnyAsync(i => i.Id == observed_entity.Id));
        }
    }
}
