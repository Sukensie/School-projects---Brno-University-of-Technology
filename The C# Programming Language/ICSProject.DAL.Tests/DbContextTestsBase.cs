using System;
using System.Threading.Tasks;
using ICSProject.Common.Tests;
using ICSProject.Common.Tests.Factories;
using ICSProject.DAL;
using ICSProject.DAL.Factories;
using Microsoft.EntityFrameworkCore;
using Xunit;
using Xunit.Abstractions;

namespace ICSProject.DAL.Tests;

public class  DbContextTestsBase : IAsyncLifetime
{
    protected DbContextTestsBase(ITestOutputHelper output)
    {
        XUnitTestOutputConverter converter = new(output);
        Console.SetOut(converter);
        
        // DbContextFactory = new DbContextTestingInMemoryFactory(GetType().Name, seedTestingData: true);
        // DbContextFactory = new DbContextLocalDBTestingFactory(GetType().FullName!, seedTestingData: true);
        DbContextFactory = new DbContextSQLiteTestingFactory(GetType().FullName!, seedTestingData: true);
        
        ICSProjectDbContextSUT = DbContextFactory.CreateDbContext();
    }

    protected IDbContextFactory<ICSProjectDbContext> DbContextFactory { get; }
    protected ICSProjectDbContext ICSProjectDbContextSUT { get; }


    public async Task InitializeAsync()
    {
        await ICSProjectDbContextSUT.Database.EnsureDeletedAsync();
        await ICSProjectDbContextSUT.Database.EnsureCreatedAsync();
    }

    public async Task DisposeAsync()
    {
        await ICSProjectDbContextSUT.Database.EnsureDeletedAsync();
        await ICSProjectDbContextSUT.DisposeAsync();
    }
}