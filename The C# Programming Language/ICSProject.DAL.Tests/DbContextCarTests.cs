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
    public class DbContextCarTests : DbContextTestsBase
    {
        public DbContextCarTests(ITestOutputHelper output) : base(output)
        {
        }

        [Fact]
        public async Task AddNew_Car_Persisted()
        {
            var entity = CarSeeds.EmptyCarEntity with
            {
                Id = Guid.Parse("E5235AF4-17BE-4EE3-ABD4-30963934FF99"),
                Manufacturer = "Nissan",
                Type = "Skyline r34",
                RegistrationTime = new DateTime(1998, 10, 1, 0, 0, 0),
                NumberOfSeats = 5,
                ImageUrl =null,
                OwnerId = UserSeeds.UserWithCar.Id
            };

            //Act
            ICSProjectDbContextSUT.Cars.Add(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntities = await dbx.Cars.SingleAsync(i => i.Id == entity.Id);

            DeepAssert.Equal(entity, actualEntities);
        }

        [Fact]
        public async Task GetById_Car_Car2Retrieved()
        {
            //Act
            var entity = await ICSProjectDbContextSUT.Cars.SingleAsync(i => i.Id == CarSeeds.Car2.Id);

            //Assert
            DeepAssert.Equal(CarSeeds.Car2 with { Owner = null, Rides = Array.Empty<RideEntity>() }, entity);
        }

        [Fact]
        public async Task GetAll_Cars_ForOwner()
        {
            //Act
            var Cars = await ICSProjectDbContextSUT.Cars
                .Where(i => i.OwnerId == UserSeeds.UserWithCar.Id)
                .ToArrayAsync();

            //Assert
            Assert.Equal(2, Cars.Length);
            for (int i = 0; i < Cars.Length; i++)
            {
                Assert.Equal(UserSeeds.UserWithCar.Id, Cars[i].OwnerId);
            }
        }


        [Fact]
        public async Task Update_Car_Persisted()
        {
            //Arrange
            var baseEntity = CarSeeds.Car1Update with { Owner = null, Rides = Array.Empty<RideEntity>()};
            var entity =
                baseEntity with
                {
                    Manufacturer = baseEntity + " UPDATED",
                    Type = "NEW TYPE"
                };

            //Act
            ICSProjectDbContextSUT.Cars.Update(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntity = await dbx.Cars.SingleAsync(i => i.Id == entity.Id);
            DeepAssert.Equal(entity, actualEntity);
        }

        [Fact]
        public async Task Delete_Car_Deleted()
        {
            //Arrange
            var baseEntity = CarSeeds.Car1Delete with {Owner = null, Rides = Array.Empty<RideEntity>()};

            //Act
            ICSProjectDbContextSUT.Cars.Remove(baseEntity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Cars.AnyAsync(i => i.Id == baseEntity.Id));
        }

        [Fact]
        public async Task DeleteById_Car_Deleted()
        {
            //Arrange
            var baseEntity = CarSeeds.Car1Delete with { Owner = null, Rides = Array.Empty<RideEntity>() };

            //Act
            ICSProjectDbContextSUT.Remove(
                ICSProjectDbContextSUT.Cars.Single(i => i.Id == baseEntity.Id));
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Users.AnyAsync(i => i.Id == baseEntity.Id));
        }

        [Fact]
        public async Task DeleteOwner_CarDeleted()
        {
            await using var dbx = await DbContextFactory.CreateDbContextAsync();

            var observed_entity = CarSeeds.Car1Delete with { Owner = null, Rides = Array.Empty<RideEntity>() };

            //this user has relation with Car1Delete
            var entity = UserSeeds.User1Delete with {Cars = Array.Empty<CarEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), DriverRides = Array.Empty<RideEntity>()};

            var Car_should_exist = await dbx.Cars.SingleAsync(i => i.Id == observed_entity.Id);
            DeepAssert.Equal(observed_entity, Car_should_exist);

            Assert.Equal(observed_entity.OwnerId, entity.Id);

            ICSProjectDbContextSUT.Users.Remove(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Car should be also removed after Owner deletion
            Assert.False(await ICSProjectDbContextSUT.Cars.AnyAsync(i => i.Id == observed_entity.Id));
        }

    }
}
