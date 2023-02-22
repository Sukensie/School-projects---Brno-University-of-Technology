using ICSProject.DAL.Entities;
using ICSProject.DAL.Seeds;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL
{
    public class ICSProjectDbContext : DbContext
    {
        private readonly bool _seedDemoData;

        public ICSProjectDbContext(DbContextOptions contextOptions, bool seedDemoData = true)
            : base(contextOptions)
        {
            _seedDemoData = seedDemoData;
        }

        public DbSet<CarEntity> Cars => Set<CarEntity>();
        public DbSet<PassengerRideEntity> PassengerRides => Set<PassengerRideEntity>();
        public DbSet<RideEntity> Rides => Set<RideEntity>();
        public DbSet<SemiStopEntity> SemiStops => Set<SemiStopEntity>();
        public DbSet<UserEntity> Users => Set<UserEntity>();

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);
            
            modelBuilder.Entity<UserEntity>(entity =>
            {
                entity.HasMany(d => d.Cars)
                    .WithOne(p => p.Owner)
                    .OnDelete(DeleteBehavior.Cascade);
                
                entity.HasMany(d => d.DriverRides)
                    .WithOne(p => p.Driver)
                    .OnDelete(DeleteBehavior.ClientCascade);
            });

            modelBuilder.Entity<RideEntity>(entity =>
            {
                entity.HasOne(d => d.Car)
                    .WithMany(p => p.Rides)
                    .OnDelete(DeleteBehavior.Cascade);
            });

            modelBuilder.Entity<PassengerRideEntity>(entity =>
            {
                entity.HasOne(d => d.Passenger)
                    .WithMany(p => p.PassengerRides)
                    .OnDelete(DeleteBehavior.Cascade);

                entity.HasOne(d => d.Ride)
                    .WithMany(p => p.PassengerRides)
                    .OnDelete(DeleteBehavior.ClientCascade);
            });

            modelBuilder.Entity<SemiStopEntity>(entity =>
            {
                entity.HasOne(d => d.Ride)
                    .WithMany(p => p.SemiStops)
                    .OnDelete(DeleteBehavior.Cascade);
            });
            
            if (_seedDemoData)
            {
                //seed
                PassengerRideSeeds.Seed(modelBuilder);
                RideSeeds.Seed(modelBuilder);
                CarSeeds.Seed(modelBuilder);
                UserSeeds.Seed(modelBuilder);
                SemiStopSeeds.Seed(modelBuilder);
            }
        }
    }
}
