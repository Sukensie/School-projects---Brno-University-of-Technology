using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace ICSProject.App.ViewModels
{
    public abstract class ViewModelBase : IViewModelBase, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged([CallerMemberName] string? propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}