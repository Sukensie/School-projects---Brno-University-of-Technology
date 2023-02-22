using System;
using System.Linq;
using System.Threading.Tasks;
using ICSProject.Common.Tests;
using ICSProject.Common.Tests.Seeds;
using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;
using Xunit;
using Xunit.Abstractions;
using Newtonsoft.Json;
using Xunit.Sdk; //pro JSON převody

namespace ICSProject.DAL.Tests
{
    /// <summary>
    /// Tests shows an example of DbContext usage when querying strong entity with no navigation properties.
    /// Entity has no relations, holds no foreign keys.
    /// </summary>
    public class DbContextUserTests : DbContextTestsBase
    {
        public DbContextUserTests(ITestOutputHelper output) : base(output)
        {
        }

        [Fact]
        public async Task AddNew_User_Persisted()
        {
            //Arrange
            var entity = UserSeeds.EmptyUserEntity with
            {
                Name = "Randy Randomer",
                Email = "randy@random.org",
                TelephoneNumber = "123456789",
                Hometown = "Březnice"
            };

            //Act
            ICSProjectDbContextSUT.Users.Add(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntities = await dbx.Users.SingleAsync(i => i.Id == entity.Id);

            DeepAssert.Equal(entity, actualEntities);
        }

        [Fact]
        public async Task GetById_User_User1Retrieved()
        {
            //Act
            var entities = await ICSProjectDbContextSUT.Users.SingleAsync(i => i.Id == UserSeeds.User1.Id);
            var expected = UserSeeds.User1 with { Cars = Array.Empty<CarEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), DriverRides = Array.Empty<RideEntity>() };
            //Assert
            DeepAssert.Equal(expected, entities); //ověření, že v common seeds je stejná entita, jako vseeds (maybe spíš v celém db contextu = všechny entity)
        }

        [Fact]
        public async Task Get_Passenger_ForPassengerRide()
        {
            //Act
            var Passengers = await ICSProjectDbContextSUT.Users
               .Where(i => i.Id == PassengerRideSeeds.PassengerRide.PassengerId)
               .ToArrayAsync();

            //Assert
            Assert.Single(Passengers);
            Assert.Equal(PassengerRideSeeds.PassengerRide.PassengerId, Passengers[0].Id);
        }

        [Fact]
        public async Task Update_User_Persisted()
        {
            //Arrange
            var baseEntity = UserSeeds.User1Update with { Cars = Array.Empty<CarEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), DriverRides = Array.Empty<RideEntity>() }; ;
            var entity =
                baseEntity with
                {
                    Name = baseEntity + " Updated",
                    Email = baseEntity + " Updated",
                };

            //Act
            ICSProjectDbContextSUT.Users.Update(entity);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntity = await dbx.Users.SingleAsync(i => i.Id == entity.Id);
            DeepAssert.Equal(entity, actualEntity);
        }

        [Fact]
        public async Task Delete_User_User1Deleted()
        {
            //Arrange
            var entityBase = UserSeeds.User1Delete with { Cars = Array.Empty<CarEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), DriverRides = Array.Empty<RideEntity>() }; 

            //Act - Database contains the user at first
            await using var dbx = await DbContextFactory.CreateDbContextAsync();
            var actualEntity = await dbx.Users.SingleAsync(i => i.Id == entityBase.Id);

            DeepAssert.Equal(entityBase, actualEntity);

            //Act - Now let's delete the user
            ICSProjectDbContextSUT.Users.Remove(entityBase);
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Users.AnyAsync(i => i.Id == entityBase.Id));
        }

        [Fact]
        public async Task DeleteById_User_User1Deleted()
        {
            //Arrange
            var entityBase = UserSeeds.User1DeleteById with { Cars = Array.Empty<CarEntity>(), PassengerRides = Array.Empty<PassengerRideEntity>(), DriverRides = Array.Empty<RideEntity>() };

            //Act
            ICSProjectDbContextSUT.Remove(
                ICSProjectDbContextSUT.Users.Single(i => i.Id == entityBase.Id));
            await ICSProjectDbContextSUT.SaveChangesAsync();

            //Assert
            Assert.False(await ICSProjectDbContextSUT.Users.AnyAsync(i => i.Id == entityBase.Id));
        }
    }
}
