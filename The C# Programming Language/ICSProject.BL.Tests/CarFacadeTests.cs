using ICSProject.BL.Models;
using System.Linq;
using System;
using System.Threading.Tasks;
using ICSProject.BL.Facades;
using ICSProject.Common.Tests;
using ICSProject.Common.Tests.Seeds;
using Microsoft.EntityFrameworkCore;
using Xunit;
using Xunit.Abstractions;

namespace ICSProject.BL.Tests
{
    public sealed class CarFacadeTests : CRUDFacadeTestsBase
    {
        private readonly CarFacade _CarFacadeSut;

        public CarFacadeTests(ITestOutputHelper output) : base(output)
        {
            _CarFacadeSut = new CarFacade(UnitOfWorkFactory, Mapper);
        }
        
        [Fact]
        public async Task Create_DetailModel_DoesNotThrow()
        {
            var model = new CarDetailModel
            (   
                Manufacturer: "Lancia",
                Type: "Fast",
                RegistrationTime: new DateTime(2020, 1, 1, 0, 0, 0),
                NumberOfSeats: 4,
                ImageUrl: default,
                OwnerId: UserSeeds.UserWithCar.Id
            );

            var _ = await _CarFacadeSut.SaveAsync(model);
        }

        [Fact]
        public async Task GetById_SeededCar1()
        {
            var Garage = await _CarFacadeSut.GetAsync(CarSeeds.Car1.Id);

            //DeepAssert.Equal(Mapper.Map<CarDetailModel>(CarSeeds.Car1), Garage);
        }

        [Fact]
        public async Task GetAll_Single_SeededCar1()
        {
            var Garages = await _CarFacadeSut.GetAsync();
                var Garage = Garages.Single(i => i.Id == CarSeeds.Car1.Id);

            DeepAssert.Equal(Mapper.Map<CarListModel>(CarSeeds.Car1), Garage);
        }
    
        [Fact]
        public async Task GetById_NonExistent()
        {
            var Garage = await _CarFacadeSut.GetAsync(CarSeeds.EmptyCarEntity.Id);
    
            Assert.Null(Garage);
        }        

        [Fact]
        public async Task NewGarage_InsertOrUpdate_CarAdded()
        {
            //Arrange
            var Garage = new CarDetailModel
            (
                Manufacturer: "Renault",
                Type: "Megan",
                RegistrationTime: new DateTime(1998, 1, 1, 0, 0, 0),
                NumberOfSeats: 5,
                ImageUrl: default,
                OwnerId: UserSeeds.UserWithCar.Id
            );

            //Act
            Garage = await _CarFacadeSut.SaveAsync(Garage);
        
            //Assert
            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            var GarageFromDb = await dbxAssert.Cars.SingleAsync(i => i.Id == Garage.Id);
            DeepAssert.Equal(Garage, Mapper.Map<CarDetailModel>(GarageFromDb));
        }
        
        [Fact]
        public async Task SeededGarage_InsertOrUpdate_GarageUpdated()
        {
            //Arrange
            var Car = new CarDetailModel
            (
                Manufacturer: CarSeeds.Car1.Manufacturer,
                Type: CarSeeds.Car1.Type,
                RegistrationTime: CarSeeds.Car1.RegistrationTime,
                NumberOfSeats: CarSeeds.Car1.NumberOfSeats,
                ImageUrl: CarSeeds.Car1.ImageUrl,
                OwnerId: CarSeeds.Car1.OwnerId
            )
            {
                Id = CarSeeds.Car1.Id
            };
            Car.Manufacturer += "updated";
            Car.Type += "updated";
            Car.NumberOfSeats = 7;

            //Act
            await _CarFacadeSut.SaveAsync(Car);
        
            //Assert
            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            var CarFromDb = await dbxAssert.Cars.SingleAsync(i => i.Id == Car.Id);
            DeepAssert.Equal(Car, Mapper.Map<CarDetailModel>(CarFromDb));
        }
        
        [Fact]
        public async Task SeededGarage_UpdateLate_ThrowError()
        {
            //Arrange
            var Car = new CarDetailModel
            (
                Manufacturer: CarSeeds.Car1.Manufacturer,
                Type: CarSeeds.Car1.Type,
                RegistrationTime: CarSeeds.Car1.RegistrationTime,
                NumberOfSeats: CarSeeds.Car1.NumberOfSeats,
                ImageUrl: CarSeeds.Car1.ImageUrl,
                OwnerId: CarSeeds.Car1.OwnerId
            )
            {
                Id = CarSeeds.Car1.Id
            };
            Car.Manufacturer += "updated";
            Car.Type += "updated";
            Car.NumberOfSeats = 7;

            //Act
            await _CarFacadeSut.SaveAsync(Car);

            //Assert
            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            var CarFromDb = await dbxAssert.Cars.SingleAsync(i => i.Id == Car.Id);

            //late update
            Car.Manufacturer += "updated";
            Assert.NotEqual(Car, Mapper.Map<CarDetailModel>(CarFromDb));
        }
        
        [Fact]
        public async Task DeleteGarage_FromSeeded_DoesNotThrow()
        {
            //Arrange
            var detailModel = Mapper.Map<CarDetailModel>(CarSeeds.Car1Delete);

            //Act & Assert
            await _CarFacadeSut.DeleteAsync(detailModel);
        }

        [Fact]
        public async Task SeededGarage_DeleteById_Deleted()
        {
            await _CarFacadeSut.DeleteAsync(CarSeeds.Car1.Id);

            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            Assert.False(await dbxAssert.Cars.AnyAsync(i => i.Id == CarSeeds.Car1.Id));
        }
    }
}
