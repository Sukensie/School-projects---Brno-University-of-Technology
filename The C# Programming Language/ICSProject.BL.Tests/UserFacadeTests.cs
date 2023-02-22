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
    public sealed class UserFacadeTests : CRUDFacadeTestsBase
    {
        private readonly UserFacade _UserFacadeSUT;

        public UserFacadeTests(ITestOutputHelper output) : base(output)
        {
            _UserFacadeSUT = new UserFacade(UnitOfWorkFactory, Mapper);
        }
        
        [Fact]
        public async Task Create_DetailModel_DoesNotThrow()
        {
            var model = new UserDetailModel
            (
                Name: "User 12",
                Email: "TestovacíUser@test.net",
                TelephoneNumber: "753159846",
                Birthday: new DateTime(2020, 1, 1, 0, 0, 0),
                Hometown: "Brno",
                ImageUrl: string.Empty
            );

            var _ = await _UserFacadeSUT.SaveAsync(model);
        }
        
        [Fact]
        public async Task GetById_SeededUser1()
        {
            var User = await _UserFacadeSUT.GetAsync(UserSeeds.User1.Id);

            DeepAssert.Equal(Mapper.Map<UserDetailModel>(UserSeeds.User1), User);
        }

        [Fact]
        public async Task GetAll_Single_SeededUser1()
        {
            var Users = await _UserFacadeSUT.GetAsync();
                var User = Users.Single(i => i.Id == UserSeeds.User1.Id);

            DeepAssert.Equal(Mapper.Map<UserListModel>(UserSeeds.User1), User);
        }
    
        [Fact]
        public async Task GetById_NonExistent()
        {
            var User = await _UserFacadeSUT.GetAsync(UserSeeds.EmptyUserEntity.Id);
    
            Assert.Null(User);
        }        
        
        [Fact]
        public async Task NewUser_InsertOrUpdate_UserAdded()
        {
            //Arrange
            var User = new UserDetailModel
            (
                Name: "Ondys Véča",
                Email: "alealeale@zleanelit.team",
                TelephoneNumber: "696969696",
                Birthday: new DateTime(2000, 9, 24),
                Hometown: "Praha",
                ImageUrl: string.Empty
            );

            //Act
            User = await _UserFacadeSUT.SaveAsync(User);
        
            //Assert
            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            var UserFromDb = await dbxAssert.Users.SingleAsync(i => i.Id == User.Id);
            DeepAssert.Equal(User, Mapper.Map<UserDetailModel>(UserFromDb));
        }
        
        [Fact]
        public async Task SeededUser_InsertOrUpdate_UserUpdated()
        {
            //Arrange
            var User = new UserDetailModel
            (
                Name: UserSeeds.User1.Name,
                Email: UserSeeds.User1.Email,
                TelephoneNumber: UserSeeds.User1.TelephoneNumber,
                Birthday: UserSeeds.User1.Birthday,
                Hometown: UserSeeds.User1.Hometown,
                ImageUrl: string.Empty
            )
            {
                Id = UserSeeds.User1.Id
            };
            User.Name += "updated";
            User.Email += "updated";
            User.TelephoneNumber += "updated";
            User.Hometown += "updated";

            //Act
            await _UserFacadeSUT.SaveAsync(User);
        
            //Assert
            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            var UserFromDb = await dbxAssert.Users.SingleAsync(i => i.Id == User.Id);
            DeepAssert.Equal(User, Mapper.Map<UserDetailModel>(UserFromDb));
        }

        [Fact]
        public async Task SeededUser_UpdateLate_ThrowError()
        {
            //Arrange
            var User = new UserDetailModel
            (
                Name: UserSeeds.User1.Name,
                Email: UserSeeds.User1.Email,
                TelephoneNumber: UserSeeds.User1.TelephoneNumber,
                Birthday: UserSeeds.User1.Birthday,
                Hometown: UserSeeds.User1.Hometown,
                ImageUrl: string.Empty
            )
            {
                Id = UserSeeds.User1.Id
            };

            //Act
            await _UserFacadeSUT.SaveAsync(User);

            //Assert
            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            var UserFromDb = await dbxAssert.Users.SingleAsync(i => i.Id == User.Id);

            //late update
            User.Name += "updated";
            Assert.NotEqual(User, Mapper.Map<UserDetailModel>(UserFromDb));
        }

        [Fact]
        public async Task DeleteUser_FromSeeded_DoesNotThrow()
        {
            //Arrange
            var detailModel = Mapper.Map<UserDetailModel>(UserSeeds.User1Delete);

            //Act & Assert
            await _UserFacadeSUT.DeleteAsync(detailModel);
        }

        [Fact]
        public async Task SeededUser_DeleteById_Deleted()
        {
            await _UserFacadeSUT.DeleteAsync(UserSeeds.User1.Id);

            await using var dbxAssert = await DbContextFactory.CreateDbContextAsync();
            Assert.False(await dbxAssert.Users.AnyAsync(i => i.Id == UserSeeds.User1.Id));
        }
    }
}
