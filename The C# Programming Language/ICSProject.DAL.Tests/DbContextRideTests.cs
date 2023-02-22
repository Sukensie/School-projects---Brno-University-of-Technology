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
    public class DbContextRideTests : DbContextTestsBase
    {
        public DbContextRideTests(ITestOutputHelper output) : base(output)
        {
        }

        [Fact]
        public async Task AddNew_Ride_Persisted()
        {

            var entity = RideSeeds.EmptyRideEntity with
            {
                Id = Guid.Parse("E5235AF4-17BE-4EE3-ABD4-30963934FF99"),
                Start = "Breclav",
                End = "Brno",
                StarTime = new DateTime(2022, 10, 1, 0, 0, 0),
                EndTime = new DateTime(2022, 10, 1, 1, 0, 0),
                DriverId = UserSeeds.Driver_NoPassengers.Id,
                CarId = CarSeeds.Car_RideWithoutPassengers.Id
            };

            //Act
            ICSProjectDbContextSUT.Rides.Add(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntities = await dbx.Rides.SingleAsync(i => i.Id == entity.Id);

            DeepAssert.Equal(entity, actualEntities);
        }

        [Fact]
        public async Task GetById_Ride_RideWithSemistopRetrieved()
        {
            //Act
            var entity = await ICSProjectDbContextSUT.Rides.SingleAsync(i => i.Id == RideSeeds.RideWithSemistop.Id);

            //Assert
            DeepAssert.Equal(RideSeeds.RideWithSemistop with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null}, entity);
        }

        [Fact]
        public async Task GetAll_Rides_ForCar()
        {
            //Act
            var Rides = await ICSProjectDbContextSUT.Rides
               .Where(i => i.CarId == CarSeeds.Car1.Id)
               .ToArrayAsync();

            //Assert
            for (int i = 0; i < Rides.Length; i++)
            {
                Assert.Equal(CarSeeds.Car1.Id, Rides[i].CarId);
            }
        }

        [Fact]
        public async Task GetAll_Rides_ForDriver()
        {
            //Act
            var Rides = await ICSProjectDbContextSUT.Rides
               .Where(i => i.DriverId == UserSeeds.UserWithCar.Id)
               .ToArrayAsync();

            //Assert
            for (int i = 0; i < Rides.Length; i++)
            {
                Assert.Equal(UserSeeds.UserWithCar.Id, Rides[i].DriverId);
            }
        }

        [Fact]
        public async Task Update_Ride_Persisted()
        {
            //Arrange
            var baseEntity = RideSeeds.RideWithoutPassenger with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };
            var entity =
                baseEntity with
                {
                    Start = baseEntity + " Updated",
                    End = baseEntity + " Updated",
                };

            //Act
            ICSProjectDbContextSUT.Rides.Update(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntity = await dbx.Rides.SingleAsync(i => i.Id == entity.Id);
            DeepAssert.Equal(entity, actualEntity);
        }

        [Fact]
        public async Task Delete_RideWithoutPassengers()
        {
            //Arrange
            var baseEntity = RideSeeds.RideWithoutPassengerDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            //Act
            ICSProjectDbContextSUT.Rides.Remove(baseEntity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == baseEntity.Id));
        }

        [Fact]
        public async Task DeleteById_Ride_Ride1Deleted()
        {
            //Arrange
            var entityBase = RideSeeds.RideWithoutPassengerDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            //Act
            ICSProjectDbContextSUT.Remove(
                ICSProjectDbContextSUT.Rides.Single(i => i.Id == entityBase.Id));
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == entityBase.Id));
        }

        [Fact]
        public async Task DeleteCar_RideDeleted()
        {
            await using var dbx = await DbContextFactory.CreateDbContextAsync();

            var observed_entity = RideSeeds.RideWithoutPassengerDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            //this car has relation with RideWithoutPassengersDelete
            var entity = CarSeeds.Car_RideWithout_Delete with { Owner = null, Rides = Array.Empty<RideEntity>() };

            var ride_should_exist = await dbx.Rides.SingleAsync(i => i.Id == observed_entity.Id);
            DeepAssert.Equal(observed_entity, ride_should_exist);

            Assert.Equal(observed_entity.CarId, entity.Id);

            ICSProjectDbContextSUT.Cars.Remove(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //ride should be also removed after Car deletion
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == observed_entity.Id));
        }

        [Fact]
        public async Task DeleteDriver_RideDeleted()
        {
            await using var dbx = await DbContextFactory.CreateDbContextAsync();

            var observed_entity = RideSeeds.RideWithoutPassengerDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            //this car has relation with RideWithoutPassengersDelete
            var entity = UserSeeds.Driver_NoPassengersDelete;

            var ride_should_exist = await dbx.Rides.SingleAsync(i => i.Id == observed_entity.Id);
            DeepAssert.Equal(observed_entity, ride_should_exist);

            Assert.Equal(observed_entity.DriverId, entity.Id);

            ICSProjectDbContextSUT.Users.Remove(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //ride should be also removed after Car deletion
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == observed_entity.Id));
        }
  
        [Fact]
        public async Task Delete_Ride_RideWithoutPassengerDeleted()
        {
            //Arrange
            var entityBase = RideSeeds.RideWithoutPassengerDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            //Act
            ICSProjectDbContextSUT.Rides.Remove(entityBase);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == entityBase.Id));
        }
        [Fact]
        public async Task Delete_Ride_RideWithSemistopDeleted()
        {
            //Arrange
            var entityBase = RideSeeds.RideWithSemistopDelete with { Car = null, SemiStops = Array.Empty<SemiStopEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), Driver = null };

            //Act
            ICSProjectDbContextSUT.Rides.Remove(entityBase);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Rides.AnyAsync(i => i.Id == entityBase.Id));
        }
    }
}
