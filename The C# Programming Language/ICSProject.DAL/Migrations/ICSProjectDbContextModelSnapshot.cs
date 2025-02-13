﻿// <auto-generated />
using System;
using ICSProject.DAL;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Infrastructure;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;

#nullable disable

namespace ICSProject.DAL.Migrations
{
    [DbContext(typeof(ICSProjectDbContext))]
    partial class ICSProjectDbContextModelSnapshot : ModelSnapshot
    {
        protected override void BuildModel(ModelBuilder modelBuilder)
        {
#pragma warning disable 612, 618
            modelBuilder
                .HasAnnotation("ProductVersion", "6.0.4")
                .HasAnnotation("Relational:MaxIdentifierLength", 128);

            SqlServerModelBuilderExtensions.UseIdentityColumns(modelBuilder, 1L, 1);

            modelBuilder.Entity("ICSProject.DAL.Entities.CarEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("uniqueidentifier");

                    b.Property<string>("ImageUrl")
                        .HasColumnType("nvarchar(max)");

                    b.Property<string>("Manufacturer")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<long>("NumberOfSeats")
                        .HasColumnType("bigint");

                    b.Property<Guid>("OwnerId")
                        .HasColumnType("uniqueidentifier");

                    b.Property<DateTime>("RegistrationTime")
                        .HasColumnType("datetime2");

                    b.Property<string>("Type")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.HasKey("Id");

                    b.HasIndex("OwnerId");

                    b.ToTable("Cars");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.PassengerRideEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("uniqueidentifier");

                    b.Property<Guid>("PassengerId")
                        .HasColumnType("uniqueidentifier");

                    b.Property<Guid>("RideId")
                        .HasColumnType("uniqueidentifier");

                    b.HasKey("Id");

                    b.HasIndex("PassengerId");

                    b.HasIndex("RideId");

                    b.ToTable("PassengerRides");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.RideEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("uniqueidentifier");

                    b.Property<Guid>("CarId")
                        .HasColumnType("uniqueidentifier");

                    b.Property<Guid>("DriverId")
                        .HasColumnType("uniqueidentifier");

                    b.Property<string>("End")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<DateTime>("EndTime")
                        .HasColumnType("datetime2");

                    b.Property<long>("Price")
                        .HasColumnType("bigint");

                    b.Property<DateTime>("StarTime")
                        .HasColumnType("datetime2");

                    b.Property<string>("Start")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.HasKey("Id");

                    b.HasIndex("CarId");

                    b.HasIndex("DriverId");

                    b.ToTable("Rides");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.SemiStopEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("uniqueidentifier");

                    b.Property<DateTime>("ArrivalTime")
                        .HasColumnType("datetime2");

                    b.Property<string>("City")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<long>("Order")
                        .HasColumnType("bigint");

                    b.Property<Guid>("RideId")
                        .HasColumnType("uniqueidentifier");

                    b.HasKey("Id");

                    b.HasIndex("RideId");

                    b.ToTable("SemiStops");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.UserEntity", b =>
                {
                    b.Property<Guid>("Id")
                        .ValueGeneratedOnAdd()
                        .HasColumnType("uniqueidentifier");

                    b.Property<DateTime>("Birthday")
                        .HasColumnType("datetime2");

                    b.Property<string>("Email")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<string>("Hometown")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<string>("ImageUrl")
                        .HasColumnType("nvarchar(max)");

                    b.Property<string>("Name")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.Property<string>("TelephoneNumber")
                        .IsRequired()
                        .HasColumnType("nvarchar(max)");

                    b.HasKey("Id");

                    b.ToTable("Users");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.CarEntity", b =>
                {
                    b.HasOne("ICSProject.DAL.Entities.UserEntity", "Owner")
                        .WithMany("Cars")
                        .HasForeignKey("OwnerId")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("Owner");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.PassengerRideEntity", b =>
                {
                    b.HasOne("ICSProject.DAL.Entities.UserEntity", "Passenger")
                        .WithMany("PassengerRides")
                        .HasForeignKey("PassengerId")
                        .OnDelete(DeleteBehavior.ClientCascade)
                        .IsRequired();

                    b.HasOne("ICSProject.DAL.Entities.RideEntity", "Ride")
                        .WithMany("PassengerRides")
                        .HasForeignKey("RideId")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("Passenger");

                    b.Navigation("Ride");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.RideEntity", b =>
                {
                    b.HasOne("ICSProject.DAL.Entities.CarEntity", "Car")
                        .WithMany("Rides")
                        .HasForeignKey("CarId")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.HasOne("ICSProject.DAL.Entities.UserEntity", "Driver")
                        .WithMany("DriverRides")
                        .HasForeignKey("DriverId")
                        .OnDelete(DeleteBehavior.ClientCascade)
                        .IsRequired();

                    b.Navigation("Car");

                    b.Navigation("Driver");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.SemiStopEntity", b =>
                {
                    b.HasOne("ICSProject.DAL.Entities.RideEntity", "Ride")
                        .WithMany("SemiStops")
                        .HasForeignKey("RideId")
                        .OnDelete(DeleteBehavior.Cascade)
                        .IsRequired();

                    b.Navigation("Ride");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.CarEntity", b =>
                {
                    b.Navigation("Rides");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.RideEntity", b =>
                {
                    b.Navigation("PassengerRides");

                    b.Navigation("SemiStops");
                });

            modelBuilder.Entity("ICSProject.DAL.Entities.UserEntity", b =>
                {
                    b.Navigation("Cars");

                    b.Navigation("DriverRides");

                    b.Navigation("PassengerRides");
                });
#pragma warning restore 612, 618
        }
    }
}
