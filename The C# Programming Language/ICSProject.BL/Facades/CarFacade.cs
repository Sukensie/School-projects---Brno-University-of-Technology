using AutoMapper;
using ICSProject.BL.Models;
using ICSProject.DAL.Entities;
using ICSProject.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.BL.Facades;

public class CarFacade : CrudFacade<CarEntity, CarListModel, CarDetailModel>
{
    public CarFacade(IUnitOfWorkFactory unitOfWorkFactory, IMapper mapper) : base(unitOfWorkFactory, mapper)
    {
    }

    public async Task<IEnumerable<CarListModel>> GetAsync(Guid id)
    {
        await using var uow = UnitOfWorkFactory.Create();

        var query = uow
            .GetRepository<CarEntity>()
            .Get()
            .Where(e => id != Guid.Empty ? e.OwnerId == id : e.OwnerId == e.OwnerId); //TODO

        return await _mapper.ProjectTo<CarListModel>(query).ToArrayAsync().ConfigureAwait(false);
    }

    public async Task<CarDetailModel> GetAsyncByCarId(Guid id)
    {
        await using var uow = UnitOfWorkFactory.Create();

        var query = uow
            .GetRepository<CarEntity>()
            .Get()
            .Where(e => id != Guid.Empty ? e.Id == id : e.Id == e.Id); //TODO

        return await _mapper.ProjectTo<CarDetailModel>(query).SingleOrDefaultAsync().ConfigureAwait(false);
    }
}