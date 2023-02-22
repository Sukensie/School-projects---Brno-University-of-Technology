using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;
using Microsoft.EntityFrameworkCore.Sqlite;

namespace ICSProject.DAL.Factories
{
    /// <summary>
    /// EF Core CLI migration generation uses this DbContext to create model and migration
    /// </summary>
    public class DesignTimeDbContextFactory : IDesignTimeDbContextFactory<ICSProjectDbContext>
    {
        public ICSProjectDbContext CreateDbContext(string[] args)
        {
            DbContextOptionsBuilder<ICSProjectDbContext> builder = new();
            builder.UseSqlServer(
                @"Data Source=(LocalDB)\MSSQLLocalDB;
                Initial Catalog = ICSProject;
                MultipleActiveResultSets = True;
                Integrated Security = True; ");

            return new ICSProjectDbContext(builder.Options);
        }
    }
}
