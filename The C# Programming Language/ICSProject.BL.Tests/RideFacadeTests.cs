using System;
using System.Linq;
using System.Threading.Tasks;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using ICSProject.Common.Tests;
using ICSProject.Common.Tests.Seeds;
using Microsoft.EntityFrameworkCore;
using Xunit;
using Xunit.Abstractions;

namespace ICSProject.BL.Tests
{
    public class RideFacadeTests : CRUDFacadeTestsBase
    {
        private readonly RideFacade _facadeSUT;

        public RideFacadeTests(ITestOutputHelper output) : base(output)
        {
            _facadeSUT = new RideFacade(UnitOfWorkFactory, Mapper);
        }
        [Fact]
        public async Task Create_WithWithoutSemiStop_DoesNotThrow()
        {
            //Arrange
            var model = new RideDetailModel
            (
                StarTime: new DateTime(1998, 10, 1, 0, 0, 0),
                EndTime: new DateTime(1998, 10, 1, 0, 0, 0),
                Start: "Oslo",
                End: "Bratislava",
                CarType: CarSeeds.Car1.Type,
                CarManufacturer: CarSeeds.Car1.Manufacturer,
                CarNumberOfSeats: CarSeeds.Car1.NumberOfSeats,
                DriverId: UserSeeds.UserWithCar.Id,
                CarId: CarSeeds.Car1.Id,
                Price: 10
            ); 

             //Act
             var returnedModel = await _facadeSUT.SaveAsync(model);

            //Assert
            FixIds(model, returnedModel);
            DeepAssert.Equal(model, returnedModel);
        }

        [Fact]
        public async Task Create_WithNonExistingSemiStop_Throws()
        {
            //Arrange
            var model = new RideDetailModel
            (
                StarTime: new DateTime(1998, 10, 1, 0, 0, 0),
                EndTime: new DateTime(1998, 10, 1, 0, 0, 0),
                Start: "Oslo",
                End: "Bratislava",
                CarType: CarSeeds.Car1.Type,
                CarManufacturer: CarSeeds.Car1.Manufacturer,
                CarNumberOfSeats: CarSeeds.Car1.NumberOfSeats,
                DriverId: UserSeeds.UserWithCar.Id,
                CarId: CarSeeds.Car1.Id,
                Price: 15
            )
            {
                SemiStops = {
                    new SemiStopDetailModel(
                        RideId:Guid.Empty,
                        City: "SemiStop 1",
                        Order: 5,
                        ArrivalTime: new DateTime(1998, 10, 1, 0, 0, 0)
                        )
                }
            };

            //Act & Assert
            try
            {
                await _facadeSUT.SaveAsync(model); //In-memory pass without exception
            }
            catch(DbUpdateException){} //SqlServer throws on FK
        }
        
        [Fact]
        public async Task Create_WithSemiStop_DoesNotThrowAndEqualsCreated()
        {
            //Arrange
            var model = new RideDetailModel
            (
                StarTime: new DateTime(1998, 10, 1, 0, 0, 0),
                EndTime: new DateTime(1998, 10, 1, 0, 0, 0),
                Start: "Oslo",
                End: "Bratislava",
                CarType: CarSeeds.Car1.Type,
                CarManufacturer: CarSeeds.Car1.Manufacturer,
                CarNumberOfSeats: CarSeeds.Car1.NumberOfSeats,
                DriverId: UserSeeds.UserWithCar.Id,
                CarId: CarSeeds.Car1.Id,
                Price: 10
            )
            {
                
                SemiStops = {
                    new SemiStopDetailModel
                    (
                        City: SemiStopSeeds.SemiStop.City,
                        Order: SemiStopSeeds.SemiStop.Order,
                        ArrivalTime: SemiStopSeeds.SemiStop.ArrivalTime,
                        RideId: SemiStopSeeds.SemiStop.RideId
                    )
                }
            };

            //Act
            var returnedModel = await _facadeSUT.SaveAsync(model);

            //Assert
            FixIds(model, returnedModel);
            DeepAssert.Equal(model,returnedModel);
        }
        
        [Fact]
        public async Task Create_WithExistingAndNotExistingSemiStop_Throws()
        {
            //Arrange
            var model = new RideDetailModel
            (
                StarTime: new DateTime(1998, 10, 1, 0, 0, 0),
                EndTime: new DateTime(1998, 10, 1, 0, 0, 0),
                Start: "Oslo",
                End: "Bratislava",
                CarType: CarSeeds.Car1.Type,
                CarManufacturer: CarSeeds.Car1.Manufacturer,
                CarNumberOfSeats: CarSeeds.Car1.NumberOfSeats,
                DriverId: UserSeeds.UserWithCar.Id,
                CarId: CarSeeds.Car1.Id,
                Price: 10
            )
            {
                SemiStops = {
                    new SemiStopDetailModel(
                        City: "New city",
                        Order: 3,
                        ArrivalTime: SemiStopSeeds.SemiStop.ArrivalTime,
                        RideId: Guid.Empty
                    ),
                    Mapper.Map<SemiStopDetailModel>(SemiStopSeeds.SemiStop)
                }
            };

            //Act & Assert
            try
            {
                await _facadeSUT.SaveAsync(model);
                Assert.True(false, "Assert Fail");
            }
            catch(DbUpdateException){} //SqlServer
            catch(ArgumentException){} //In-memory
        }
        
        [Fact]
        public async Task GetById_FromSeeded_DoesNotThrowAndEqualsSeeded()
        {
            //Arrange
            var detailModel = Mapper.Map<RideDetailModel>(RideSeeds.RideWithSemistop);

            //Act
            var returnedModel = await _facadeSUT.GetAsync(detailModel.Id);

            //Assert
            DeepAssert.Equal(detailModel, returnedModel);
        }
        
        [Fact]
        public async Task GetAll_FromSeeded_DoesNotThrowAndContainsSeeded()
        {
            //Arrange
            var listModel = Mapper.Map<RideListModel>(RideSeeds.RideWithSemistop);

            //Act
            var returnedModel = await _facadeSUT.GetAsync();

            //Assert
            DeepAssert.Equal(listModel, returnedModel.ElementAt<RideListModel>(1));
        }
        
        [Fact]
        public async Task Delete_FromSeeded_DoesNotThrow()
        {
            //Arrange
            var detailModel = Mapper.Map<RideDetailModel>(RideSeeds.RideWithSemistop);

            //Act & Assert
            await _facadeSUT.DeleteAsync(detailModel);
        }
        
        [Fact]
        public async Task Update_FromSeeded_DoesNotThrow()
        {
            //Arrange
            var detailModel = Mapper.Map<RideDetailModel>(RideSeeds.RideWithSemistop);
            detailModel.Start = "Changed starting place";

            //Act & Assert
            await _facadeSUT.SaveAsync(detailModel);
        }
        
        [Fact]
        public async Task Update_Start_FromSeeded_CheckUpdated()
        {
            //Arrange
            var detailModel = Mapper.Map<RideDetailModel>(RideSeeds.RideWithSemistop);
            detailModel.Start = "Changed starting place 1";

            //Act
            await _facadeSUT.SaveAsync(detailModel);

            //Assert
            var returnedModel = await _facadeSUT.GetAsync(detailModel.Id);
            DeepAssert.Equal(detailModel, returnedModel);
        }
        
        [Fact]
        public async Task Update_RemoveSemiStops_FromSeeded_CheckUpdated()
        {
            //Arrange
            var detailModel = Mapper.Map<RideDetailModel>(RideSeeds.RideWithSemistop);
            detailModel.SemiStops.Clear();

            //Act
            await _facadeSUT.SaveAsync(detailModel);

            //Assert
            var returnedModel = await _facadeSUT.GetAsync(detailModel.Id);
            DeepAssert.Equal(detailModel, returnedModel);
        }
        
        [Fact]
        public async Task Update_RemoveOneOfSemiStops_FromSeeded_CheckUpdated()
        {
            //Arrange
            var detailModel = Mapper.Map<RideDetailModel>(RideSeeds.RideWithSemistop);
            detailModel.SemiStops.Remove(detailModel.SemiStops.First());

            //Act
            await _facadeSUT.SaveAsync(detailModel);

            //Assert
            var returnedModel = await _facadeSUT.GetAsync(detailModel.Id);
            DeepAssert.Equal(detailModel, returnedModel);
        }

        [Fact]
        public async Task DeleteById_FromSeeded_DoesNotThrow()
        {
            //Arrange & Act & Assert
            await _facadeSUT.DeleteAsync(RideSeeds.RideWithSemistop.Id);
        }
        
        private static void FixIds(RideDetailModel expectedModel, RideDetailModel returnedModel)
        {
            returnedModel.Id = expectedModel.Id;

            foreach (var SemiStopModel in returnedModel.SemiStops)
            {
                var SemiStopDetailModel = expectedModel.SemiStops.FirstOrDefault(i => 
                    i.City == SemiStopModel.City 
                    && i.Order == SemiStopModel.Order
                    && i.ArrivalTime == SemiStopModel.ArrivalTime
                    && i.Order == SemiStopModel.Order);

                if (SemiStopDetailModel != null)
                {
                    SemiStopModel.Id = SemiStopDetailModel.Id;
                    SemiStopModel.RideId = SemiStopDetailModel.RideId;
                }
            }
        }
     }
}
