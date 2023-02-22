using ICSProject.DAL;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.Common.Tests.Factories;

public class DbContextLocalDBTestingFactory : IDbContextFactory<ICSProjectDbContext>
{
    private readonly string _databaseName;
    private readonly bool _seedTestingData;

    public DbContextLocalDBTestingFactory(string databaseName, bool seedTestingData = false)
    {
        _databaseName = databaseName;
        _seedTestingData = seedTestingData;
    }
    public ICSProjectDbContext CreateDbContext()
    {
        DbContextOptionsBuilder<ICSProjectDbContext> builder = new();
        builder.UseSqlServer($"Data Source=(LocalDB)\\MSSQLLocalDB;Initial Catalog = {_databaseName};MultipleActiveResultSets = True;Integrated Security = True; ")
            .EnableSensitiveDataLogging();
        
        // contextOptionsBuilder.LogTo(System.Console.WriteLine); //Enable in case you want to see tests details, enabled may cause some inconsistencies in tests
        // builder.EnableSensitiveDataLogging();
        
        return new ICSProjectTestingDbContext(builder.Options, _seedTestingData);
    }
}