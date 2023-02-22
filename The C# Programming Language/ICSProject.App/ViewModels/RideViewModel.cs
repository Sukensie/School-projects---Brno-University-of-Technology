using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Collections.Generic;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using ICSProject.App.Extensions;
using ICSProject.App.Services;
using ICSProject.App.Messages;
using ICSProject.App.Wrappers;
using ICSProject.App.Commands;
using System.Text;
using ICSProject.App.Views;

namespace ICSProject.App.ViewModels
{

public class RideViewModel : ViewModelBase, IRideViewModel
        {
            public RideViewModel(
                RideSearchListViewModel rideSearchListViewModel,
                MenuViewModel menuViewModel
            )
            {
                MenuViewModel = menuViewModel;
                RideSearchListViewModel = rideSearchListViewModel;
            }
            private bool _isSelected;
        public bool IsSelected
        {
            get
            {
                return _isSelected;
            }
            set
            {
                _isSelected = value;
                OnPropertyChanged(nameof(IsSelected));
            }
        }

        private string _start;
        public string Start
        {
            get
            {
                return _start;
            }
            private set
            {
                _start = value;
                OnPropertyChanged(nameof(Start));
            }
        }

        private string _end;
        public string End
        {
            get
            {
                return _end;
            }
            private set
            {
                _end = value;
                OnPropertyChanged(nameof(End));
            }
        }

        private string _time;
        public string Time
        {
            get
            {
                return _time;
            }
            private set
            {
                _time = value;
                OnPropertyChanged(nameof(Time));
            }
        }

        private string _date;
        public string Date
        {
            get
            {
                return _date;
            }
            private set
            {
                _date = value;
                OnPropertyChanged(nameof(Date));
            }
        }

       





        /*

        public async Task LoadAsync()
        {
            Users.Clear();
            var users = await _rideFacade.GetAsync();
            Users.AddRange(users);
        }*/
        public MenuViewModel MenuViewModel { get; set; }
        public RideSearchListViewModel RideSearchListViewModel { get; set; }
    }
}