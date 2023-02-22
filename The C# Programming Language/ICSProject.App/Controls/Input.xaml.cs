using System.Windows;
using System.Windows.Input;

namespace ICSProject.App.Controls
{
    public partial class Input
    {
        public ICommand Cmd
        {
            get => (ICommand)GetValue(CmdProperty);
            set => SetValue(CmdProperty, value); 
        }

        public string Placeholder
        {
            get => (string)GetValue(PlaceholderProperty);
            set => SetValue(PlaceholderProperty, value); 
        }

        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }


        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(Input),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty PlaceholderProperty = DependencyProperty.Register(
            nameof(Placeholder),
            typeof(string),
            typeof(Input),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty CmdProperty = DependencyProperty.Register(
            nameof(Cmd),
            typeof(ICommand),
            typeof(Input),
            new PropertyMetadata(default(ICommand)));

        public Input()
        {
            InitializeComponent();
        }
    }
}
