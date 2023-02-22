//COMMON SEEDS
using ICSProject.DAL;
using ICSProject.Common.Tests.Seeds;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests
{
    public class ICSProjectTestingDbContext : ICSProjectDbContext
    {
        private readonly bool _seedTestingData;

        public ICSProjectTestingDbContext(DbContextOptions contextOptions, bool seedTestingData = true)
            : base(contextOptions, seedDemoData: false)
        {
            _seedTestingData = seedTestingData;
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            if (_seedTestingData)
            {
                UserSeeds.Seed(modelBuilder);
                CarSeeds.Seed(modelBuilder);
                PassengerRideSeeds.Seed(modelBuilder);
                RideSeeds.Seed(modelBuilder);
                SemiStopSeeds.Seed(modelBuilder);
            }
        }
    }
}
