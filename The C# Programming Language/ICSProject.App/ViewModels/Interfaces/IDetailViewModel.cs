using System;
using System.Threading.Tasks;

namespace ICSProject.App.ViewModels
{
    public interface IDetailViewModel<out TDetail> : IViewModelBase
    {
        TDetail? Model { get; }
        Task LoadAsync(Guid id);
        Task DeleteAsync();
        Task SaveAsync();
    }
}