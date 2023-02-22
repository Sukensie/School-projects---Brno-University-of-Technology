using AutoMapper;
using ICSProject.BL.Models;
using ICSProject.DAL.Entities;
using ICSProject.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.BL.Facades;

public class RideFacade : CrudFacade<RideEntity, RideListModel, RideDetailModel>
{
    public RideFacade(IUnitOfWorkFactory unitOfWorkFactory, IMapper mapper) : base(unitOfWorkFactory, mapper)
    {
    }
    public async Task<IEnumerable<RideListModel>> GetAsync(string start, string end, DateTime startTime)
    {
        await using var uow = UnitOfWorkFactory.Create();
        var query = uow
            .GetRepository<RideEntity>()
            .Get()
            .Where(e => (start == "" && end == "") ? e.Start == null : (start != "" ? e.Start.Contains(start) : e.Start == e.Start))
            .Where(e => end != "" ? e.End.Contains(end) : e.End == e.End)
            .Where(e => (DateTime.Compare(e.StarTime,startTime) >= 0) ? e.StarTime == e.StarTime : e.StarTime == null);
        return await _mapper.ProjectTo<RideListModel>(query).ToArrayAsync().ConfigureAwait(false);
    }

    //vratí jízdy k zadanému id (uživatele)
    public async Task<IEnumerable<RideDetailModel>> GetAsyncById(Guid id)
    {
        await using var uow = UnitOfWorkFactory.Create();

        var query = uow
            .GetRepository<RideEntity>()
            .Get()
            .Where(e => id != Guid.Empty ? e.DriverId == id : e.DriverId == e.DriverId); //TODO

        return await _mapper.ProjectTo<RideDetailModel>(query).ToArrayAsync().ConfigureAwait(false);
    }

    public async Task<IEnumerable<RideDetailModel>> GetAsyncByRideId(Guid id)
    {
        await using var uow = UnitOfWorkFactory.Create();

        var query = uow
            .GetRepository<RideEntity>()
            .Get()
            .Where(e => id != Guid.Empty ? e.Id == id : e.Id == e.Id); //TODO

        return await _mapper.ProjectTo<RideDetailModel>(query).ToArrayAsync().ConfigureAwait(false);
    }

    public async Task<IEnumerable<PassengerRideDetailModel>> GetAsyncPassengerById(Guid id)
    {
        await using var uow = UnitOfWorkFactory.Create();

        var query = uow
            .GetRepository<PassengerRideEntity>()
            .Get()
            .Where(e => id != Guid.Empty ? e.PassengerId == id : e.PassengerId == e.PassengerId); //TODO

        return await _mapper.ProjectTo<PassengerRideDetailModel>(query).ToArrayAsync().ConfigureAwait(false);
    }
}