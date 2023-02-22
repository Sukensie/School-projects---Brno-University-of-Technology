using System;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.DAL.UnitOfWork;

public class UnitOfWorkFactory : IUnitOfWorkFactory
{
    private readonly IDbContextFactory<ICSProjectDbContext> _dbContextFactory;

    public UnitOfWorkFactory(IDbContextFactory<ICSProjectDbContext> dbContextFactory)
    {
        _dbContextFactory = dbContextFactory;
    }
    public IUnitOfWork Create() => new UnitOfWork(_dbContextFactory.CreateDbContext());
}