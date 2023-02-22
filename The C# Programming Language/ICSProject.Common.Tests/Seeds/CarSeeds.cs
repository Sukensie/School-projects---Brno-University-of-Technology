using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSProject.DAL.Entities;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests.Seeds
{
    public static class CarSeeds
    {
        public static readonly CarEntity EmptyCarEntity = new(
            Id: default,
            Manufacturer: default!,
            Type: default!,
            RegistrationTime: default,
            NumberOfSeats: default,
            ImageUrl: default,
            OwnerId: default
            );

        static CarSeeds()
        {
            Car1.Rides.Add(RideSeeds.RideWithSemistop);
            Car1.Rides.Add(RideSeeds.RideWithSemistopDelete);
            Car_RideWithoutPassengers.Rides.Add(RideSeeds.RideWithoutPassenger);
            Car_RideWithout_Delete.Rides.Add(RideSeeds.RideWithoutPassengerDelete);
            Car_RideWithPassengers.Rides.Add(RideSeeds.RideWithPassenger);
            Car_RideWith_Delete.Rides.Add(RideSeeds.RideWithPassengerDelete);
        }

        public static readonly CarEntity Car1 = new(
            Id: Guid.Parse(input: "F1C4DEFC-F19D-4EBF-B5E1-127CA80D458E"),
            Manufacturer: "Car1 seeded manufacturer",
            Type: "Car1 seeded Type",
            RegistrationTime: new DateTime(2020, 1, 1, 0, 0, 0),
            NumberOfSeats: 5,
            ImageUrl: null,
            OwnerId: UserSeeds.UserWithCar.Id)
        {
            Owner = UserSeeds.UserWithCar
        };

        public static readonly CarEntity Car2 = new(
            Id: Guid.Parse(input: "F9A315C7-FE39-4182-B862-A4F5C5102488"),
            Manufacturer: "Car2 seeded manufacturer",
            Type: "Car2 seeded Type",
            RegistrationTime: new DateTime(2008, 1, 1, 0, 0, 0),
            NumberOfSeats: 3,
            ImageUrl: null,
            OwnerId: UserSeeds.UserWithCar.Id)
        {
            Owner = UserSeeds.UserWithCar
        };

        public static readonly CarEntity Car_RideWithoutPassengers = new(
            Id : Guid.Parse("526980EE-A853-4081-A325-15FFCBE681B8"),
            Manufacturer : "Car_RideWithoutPassengers manufacturer",
            Type : "Car_RideWithoutPassengers type",
            OwnerId:UserSeeds.Driver_NoPassengers.Id,
            NumberOfSeats: 5,
            RegistrationTime: new DateTime(2018,1,1,0,0,0),
            ImageUrl: null
        )
        {
            Owner = UserSeeds.Driver_NoPassengers
        };

        public static readonly CarEntity Car_RideWithout_Delete = new(
            Id : Guid.Parse("2D50E393-C694-4024-B988-02EC1C55C7C4"),
            Manufacturer: "Car_RideWithoutPassengers manufacturer",
            Type: "Car_RideWithoutPassengers type",
            OwnerId : UserSeeds.Driver_NoPassengersDelete.Id,
            NumberOfSeats: 5,
            RegistrationTime: new DateTime(2018, 1, 1, 0, 0, 0),
            ImageUrl: null
        )
        {
            Owner = UserSeeds.Driver_NoPassengersDelete
        };

        public static readonly CarEntity Car_RideWithPassengers = new(
            Id : Guid.Parse("96172F9F-7335-4D62-ADB2-B043BC548493"),
            Manufacturer : "Car_RideWithPassengers manufacturer",
            Type : "Car_RideWithPassengers type",
            OwnerId : UserSeeds.Driver_WithPassengers.Id,
            RegistrationTime: default,
            NumberOfSeats: 5,
            ImageUrl: null
        )
        {
            Owner = UserSeeds.Driver_WithPassengers
        };
       
        public static readonly CarEntity Car_RideWith_Delete = new(
            Id : Guid.Parse("56657E92-0601-4EB3-8BB1-355C880AE97E"),
            OwnerId : UserSeeds.Driver_WithPassengersDelete.Id,
            Manufacturer: "Manufacturer RideWith_delete",
            Type: "Type RideWith_delete",
            RegistrationTime: default,
            NumberOfSeats: 5,
            ImageUrl: null
            )
        {
            Owner = UserSeeds.Driver_WithPassengersDelete
        };

        public static readonly CarEntity Car1Update = new(
            Id : Guid.Parse("167069E9-ACB3-4274-B6BB-65E5B052EC00"),
            OwnerId : UserSeeds.User1Update.Id,
            Manufacturer: "Car1 seeded manufacturer",
            Type: "Car1 seeded Type",
            RegistrationTime: new DateTime(2020, 1, 1, 0, 0, 0),
            NumberOfSeats: 5,
            ImageUrl: null)
        {
            Owner = UserSeeds.User1Update
        };

        public static readonly CarEntity Car1Delete = new (
            Id : Guid.Parse("CFF44A8C-C182-4BF7-94C5-DB59A649D839"),
            OwnerId : UserSeeds.User1Delete.Id,
            Manufacturer: "Car1 seeded manufacturer",
            Type: "Car1 seeded Type",
            RegistrationTime: new DateTime(2020, 1, 1, 0, 0, 0),
            NumberOfSeats: 5,
            ImageUrl: null

        )
    {
        Owner = UserSeeds.User1Delete
    };
        public static void Seed(this ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<CarEntity>().HasData(
                Car1 with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car2 with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car1Update with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car1Delete with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car_RideWithPassengers with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car_RideWithoutPassengers with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car_RideWith_Delete with { Owner = null, Rides = Array.Empty<RideEntity>() },
                Car_RideWithout_Delete with { Owner = null, Rides = Array.Empty<RideEntity>() }
                );
        }
    }
}
