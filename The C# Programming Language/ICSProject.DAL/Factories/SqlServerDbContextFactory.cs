using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.Factories
{
    public class SqlServerDbContextFactory : IDbContextFactory<ICSProjectDbContext>
    {
        private readonly string _connectionString;
        private readonly bool _seedDemoData;

        public SqlServerDbContextFactory(string connectionString, bool seedDemoData = false)
        {
            _connectionString = connectionString;
            _seedDemoData = seedDemoData;
        }

        public ICSProjectDbContext CreateDbContext()
        {
            var optionsBuilder = new DbContextOptionsBuilder<ICSProjectDbContext>();
            optionsBuilder.UseSqlServer(_connectionString);

            //optionsBuilder.LogTo(System.Console.WriteLine); //Enable in case you want to see tests details, enabled may cause some inconsistencies in tests
            //optionsBuilder.EnableSensitiveDataLogging();

            return new ICSProjectDbContext(optionsBuilder.Options, _seedDemoData);
        }
    }
}