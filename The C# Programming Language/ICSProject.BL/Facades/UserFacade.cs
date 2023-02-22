using AutoMapper;
using ICSProject.BL.Models;
using ICSProject.DAL.Entities;
using ICSProject.DAL.UnitOfWork;
using Microsoft.EntityFrameworkCore;

namespace ICSProject.BL.Facades;

public class UserFacade : CrudFacade<UserEntity, UserListModel, UserDetailModel>
{
    public UserFacade(IUnitOfWorkFactory unitOfWorkFactory, IMapper mapper) : base(unitOfWorkFactory, mapper)
    {
    }

    public async Task<IEnumerable<UserListModel>> GetAsync(string name)
    {
        await using var uow = UnitOfWorkFactory.Create();
        var query = uow
            .GetRepository<UserEntity>()
            .Get()
            .Where(e => (name != "" ?  e.Name.Contains(name) : e.Name == e.Name)); //TODO


        return await _mapper.ProjectTo<UserListModel>(query).ToArrayAsync().ConfigureAwait(false);
    }
}